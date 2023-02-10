什麼是現場可編程輯閘陣列(Field-Programmable Gate Arrays；FPGA)？

FPGA是一種可以重複改變組態的電路，可使用者進行編程的邏輯閘元件，
特別適用於產品開發時必須不斷變更設計的應用，以有效加速產品上市時間
。FPGA的邏輯閘特性，可依設計者的需要加以改變，並提供各種基本功能。
然而，一般而言，FPGA比DSP更為昂貴，所消耗的功率也較高；
FPGA的速度也較ASIC更慢，無法完成複雜的設計，因此FPGA常與DSP或ASIC搭配使用，以提供更好的彈性、價格性能比與電源需求。



cIOSMIOManager is a top node,it contain driver interface and all device,
driver interface contain all IO data list,



the purpose to update all device and IO is go as follow

//first to call all IOSM data upate to setup singnal
//second to call device update
//
//so the IOSMIOData could set status to back if necessary
//and Device Behavior won't set IOSMIO data incorrectly.

void	cIOSMIOManager::Update(float e_fElpaseTime);