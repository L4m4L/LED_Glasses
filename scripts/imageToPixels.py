# imageToPixels
# Take an image and output the 7x5 array of RGB values 

from PIL import Image
import pprint
import sys

pp = pprint.PrettyPrinter(indent=4)

im = Image.open('Images/custom.png', 'r')

NUMCOLS=5
NUMROWS=7

im = im.resize((NUMCOLS, NUMROWS), Image.ANTIALIAS)

pixels=list(im.getdata())

sys.stdout = open ("imagePixels.txt", "w")

for index, pixel in enumerate(pixels):
	if(index==0):
		print('{',end='')
	for colour in range(0, 3):
		print(pixel[colour], end=", ")
	index+=1
	if (index % NUMCOLS == 0):
		print('},')
		print('{',end='')

sys.stdout.close()