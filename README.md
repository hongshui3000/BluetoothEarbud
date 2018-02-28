# BluetoothEarbud
Study new Qualcomm QCC3026 platform for Earbud(TWS)
### Default supported peripherals
This QCC3026 platform is CF376_CF440, it do not support Proximity(vncl3020) and Accelerometer(adxl362)  
If we need to add other sensor, we should add driver code  
Add Proximity via INCLUDE_PROXIMITY && HAVE_VNCL3020  
Add Accelermeter via INCLUDE_ACCELEROMETER && HAVE_ADXL362  
  
Then, in QMDE, open Project, in Project Mode, add the define in DEFS Tab
