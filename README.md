This branch "platform_dev_drv" contains the code for creating dummy platform device
if you insert the driver first and then see the dmesg you will see only the driver insertion message but when you insert the device module and run dmesg you will see the probe functions are called and device is detected message appears, it confirms that the device was not available first and hence the driver couldn't match the device name, which is normally comming from device tree but here we simulating in c code with creating separate device module, and right after inerstion of device module the probe functions are called 
