Jan 27th, 2021 I got 4 XIAO's working with my TensorflowLIt Hello World sine wave. Here is the code.



-01 are basic I2C code
-02 I have included my Version of the TensorflowLite Hello World Sine Wave. These are simplified and all in one file including the Tensorflow Micro model.





For the slave program line 146 should reflect the number assigned to the controlled board
```

Wire.begin(1); 
```

Line 505 should say what the board is
```
 Wire.write( "I am one .." );
```

Note I had troubles sending comments of different lengths, so expansion of the code might be tricky.


Here is an image of the setup. I used 16k Ohm resistors however I am sure someone else knows what is the best resistor to use depending on how many XIAOs you have attached.

![20210128_090622.jpg](20210128_090622.jpg)

