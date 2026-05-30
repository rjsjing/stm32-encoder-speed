# CLAUDE.md — STM32F103C8T6 编码器接口测速

## 技术栈

- **MCU**: STM32F103C8T6（Cortex-M3, 64KB Flash, 20KB SRAM, 72MHz）
- **库**: ST 标准外设库 v3.5.0
- **构建工具**: Keil MDK uVision5 + EIDE (VSCode 插件 `cl.eide`)
- **调试器**: ST-Link (SWD)

## 目录结构

```
├── Start/           CMSIS 启动 + system_stm32f10x
├── Library/         ST 标准外设库 v3.5.0
├── System/          Delay.c/h — SysTick 延时
│                    Timer.c/h — TIM2 定时中断
├── Hardwera/        自定义硬件驱动
│   ├── Encoder.c/h  ← TIM3 编码器接口 (PA6/PA7)
│   └── OLED.c/h     ← OLED 显示驱动
├── User/            main.c, stm32f10x_conf.h, stm32f10x_it.c/h
└── build/Objects/Listings/
```

## 硬件引脚映射

| 引脚 | 功能 | 连接目标 |
|------|------|----------|
| PA6 | TIM3_CH1 IPU | 编码器 A 相 |
| PA7 | TIM3_CH2 IPU | 编码器 B 相 |
| PB5 | GPIO Out OD | OLED SCL |
| PB6 | GPIO Out OD | OLED SDA |

## 驱动模块

### Encoder (Hardwera/Encoder.c/h) — TIM3 编码器接口
- `Encoder_Init()` — TIM3 编码器模式 TI12, CH1+CH2 双边沿计数
  - ARR=65535, PSC=0, 滤波器 0xF
  - 编码器模式: 上升沿触发 (不反相)
- `Encoder_Get()` — 读取计数器并清零，返回速度增量

### Timer (System/Timer.c/h)
- `Timer2_Init(65535, 0)` — TIM2 定时中断刷新速度值

## 功能说明

TIM3 编码器接口模式读取 A/B 相正交信号，自动处理方向判断和计数。TIM2 定时中断读取并清零编码器计数值，作为速度值显示在 OLED 上。

## 编码规范
- 函数命名: `模块名_操作()`
- 编码: UTF-8，注释用中文

## Git 规范
```
main              ← 稳定分支
提交: feat:/fix:/docs: 格式
```

## 构建命令
```
Keil:  打开 Project.uvprojx → F7 编译 → F8 烧录
EIDE:  Ctrl+Shift+P → "EIDE: Build"
```

## 注意事项
- `USE_STDPERIPH_DRIVER` + `STM32F10X_MD` 必须定义
- 启动文件 `startup_stm32f10x_md.s`
- TIM3 编码器模式自动处理计数方向，无需手动判断 A/B 相电平
