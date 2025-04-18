# LiteLdacsCrypto

为LiteLdacs提供安全支持，适用于libldcauc及ldacs-combine

---

## 1. 依赖

### 对于尚未使用密码卡的设备
下载并安装GMSSL国密库
```shell
git clone https://github.com/guanzhi/GmSSL.git
cd GmSSL && mkdir build && cd build
cmake .. && make -j12 && sudo make install
sudo ldconfig
```


### **对于AS、GS设备**
使用piico-manager密码卡工具安装依赖库
```shell
git clone xxxx
```
### **对于SGW设备**
请直接使用提供的网关工控机


## 2. 安装

### 下载
```shell
git clone https://github.com/liteldacs/liteldacscrypto.git
cd liteldacscrypto && mkdir build && cd build
```

### **对于尚未使用密码卡的设备**
```shell
cmake ..
```
### **对于AS设备**
```shell
cmake .. -DAS_DEVICE=ON
```
### **对于GS设备**
```shell
cmake .. -DGS_DEVICE=ON
```
### **对于SGW设备**
```shell
cmake .. -DSGW_DEVICE=ON
```

进行编译及安装
```shell
make -j12 & sudo make install
```
