import serial, os
from PIL import Image

w=240
h=150

wn=640
hn=400

ser=serial.Serial(port="COM25",baudrate=115200*20,parity=serial.PARITY_NONE,stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS,timeout=1)

print("ready")

s=""
while True:
	while s!=b"OwO":
		s=ser.read(3)
	
	s=ser.read(w*h*2)
	
	img=Image.new("RGB",(w,h))
	img.putdata([((s[i]&0b11111)*255//32, ((((s[i+1]<<8)+s[i])>>5)&0b111111)*255//64, (s[i+1]>>3)*255//32) for i in range(0,w*h*2,2)])
	
	#img=img.resize((wn,hn))
	
	n=0
	while os.path.isfile(str(n)+".png"):
		n+=1
	img.save(str(n)+".png")
	
	print("saved screenshot %d"%n)