
该工程是通过Onmicro的私有2.4G来模拟BLE的广播的demo。

其特性如下：
- 只实现了TX转RX，即：发ADV_XXX_IND包，然后收SCAN_REQ包，不能再发送SCAN_RSP；
- 受限于2.4G最长包长为32bytes的限制，广播包TX最长payload长度为27bytes，RX最长payload长度为26bytes；
- BLE的CRC和白化均为软件实现；
- 私有2.4G工作时会强行把PHY切到私有2.4G模块，工作完成会再将PHY切回给BLE模块；
- 可实现2402到2481任意奇偶数频点的配置；
- 可任意配置BLE的AA/header/payload；

重要文件说明：
- `hw_transceiver.c`: 私有2.4G主要函数封装接口；
- `tr_ble_sim.c`: 私有2.4G模拟BLE的核心实现；
- `app_tr_ble_sim.c`: 提供基本BLE收发的Demo；

工程基本说明：
- 这个工程是基于“ble_app_central_and_peripheral”工程创建的，其蓝牙通过函数`app_uart1_rx_callback`来用UART控制；
- 私有2.4G Demo工作流程：GPIO2边沿中断 -> 计算GPIO2延时损失 -> 通过DWT来定时2.4G的TX时间点 -> 进行连续10次的广播TX转RX的收发；
- 私有2.4G Demo的广播内容：type=ADV_IND, bdaddr=0x01BF66212424, freq=2402MHz, name="XX"；
- 可将 `hw_transceiver.c`, `tr_ble_sim.c`, `app_tr_ble_sim.c` 三个文件放到SRAM中执行以提高其执行速度和执行延时；

基本测试数据（`hw_transceiver.c`, `tr_ble_sim.c`, `app_tr_ble_sim.c`放到SRAM中）：
- 通过示波器余晖测试，从GPIO2来上升沿，到2.4G的TXEN(默认GPIO12)拉起的抖动时间为65ns。GPIO通过64MHz的timer的capture采集；
- 连续2个未收到SCAN_REQ的包的最小间隔：648us，包括152us的软件准备TX数据时间（白化，CRC，位翻转，SPI），如果客户TX数据可以提前准备好，可以更短；

