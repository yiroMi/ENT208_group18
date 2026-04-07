#include "stm32f10x.h"      // STM32底层库
#include "Delay.h"          // 延时函数
#include "led.h"            // LED控制
#include "servo.h"          // 舵机控制（内部已集成PWM）
#include "key.h"            // 按键模块
#include "usart.h"          // 串口通信（语音）
#include "fire.h"           // 火焰检测
#include "buzzer.h"         // 蜂鸣器

// ================= 全局变量 =================

// 按键返回值（1~4）
uint8_t key_val = 0;

// 当前选择的垃圾桶编号（1~4）
uint8_t waste_val = 0;

// 火焰检测标志（0=无火，1=有火）
int8_t fire = 0;


// ================= 主函数 =================
int main()
{
    // ---------- 硬件初始化 ----------

    led_init();         // 初始化LED（用于满溢提示）
    Servo_Init();       // 初始化舵机（内部完成PWM配置）
    key_init();         // 初始化按键
    buzzer_init();      // 初始化蜂鸣器

    Usart2_Init(9600);  // 串口2

    // ---------- 舵机初始位置 ----------
    // 将4个垃圾桶全部复位到“关闭状态”
    servo_setangle1(90);
    servo_setangle2(90);
    servo_setangle3(90);
    servo_setangle4(90);


    // ================= 主循环 =================
    while(1)
    {
        // ---------- 1. 满溢检测 ----------
        // 检测垃圾桶是否已满
        // 若满 → 内部会控制LED点亮
        Overflow();


        // ---------- 2. 语音控制 ----------
        // 语音模块识别结果（如“瓶子”）
        // 转换为对应垃圾桶编号 → waste_val
        voice_function();


        // ---------- 3. 按键控制 ----------
        // 读取按键编号（1~4）
        key_val = key_getnum();

        // 将按键输入映射为垃圾桶编号
        if(key_val == 1)
        {
            waste_val = 1;
        }
        else if(key_val == 2)
        {
            waste_val = 2;
        }
        else if(key_val == 3)
        {
            waste_val = 3;
        }
        else if(key_val == 4)
        {
            waste_val = 4;
        }


        // ---------- 4. 火焰检测 + 报警 ----------
        // 检测是否有火焰
        fire = get_fire();

        if(fire != 0)
        {
            buzzer_play();   // 有火 → 蜂鸣器报警
        }
        else
        {
            buzzer_stop();   // 无火 → 关闭蜂鸣器
        }


        // ---------- 5. 垃圾桶开盖控制 ----------
        // 根据 waste_val 控制对应垃圾桶
        switch(waste_val)
        {
            case 1:
                servo_1();     // 打开1号垃圾桶
                waste_val = 0; // 执行后清零，避免重复执行
                break;

            case 2:
                servo_2();     // 打开2号垃圾桶
                waste_val = 0;
                break;

            case 3:
                servo_3();     // 打开3号垃圾桶
                waste_val = 0;
                break;

            case 4:
                servo_4();     // 打开4号垃圾桶
                waste_val = 0;
                break;

            default:
                break;   // 没有输入时不执行任何动作
        }


        // ---------- 6. 循环延时 ----------
        // 防止CPU占用过高，同时起到简单“节流”作用
        Delay_ms(100);
    }
}