## platform device:
Platform devices in Linux kernel systems are those that cannot be automatically discovered by the system (unlike PCI or USB devices). Instead, they need to be explicitly initialized and registered manually in the kernel code, with the initialization process triggered by the kernel during boot or by specific drivers.

## platform driver:
Platform driver detect the relevant device and interact with it, here the basic driver implemented and currently it only matches the first device and when match successful, the probe function is called and displayed message "device detected"
