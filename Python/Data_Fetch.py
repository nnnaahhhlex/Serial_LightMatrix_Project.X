# Read the csv file
# Get current change in price for a symbole
# Multiply the current price change by share quantity
# Store the value
# for all rows (excluding top row) repear until no more



import requests
import bs4
import csv
import serial
from time import sleep

ser = serial.Serial(port='COM5',baudrate=9600,parity=serial.PARITY_NONE,stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS,timeout=1)
print("connected to: " + ser.portstr)
print('\n')

# opening the CSV file
with open('Portfolio.csv', mode ='r') as file:
   
  # reading the CSV file
  csv_data = csv.reader(file)
  data_lines = list(csv_data)

  # displaying the contents of the CSV file
  portfolio_change = 0
  for lines in data_lines[1:]:
  	
  	symbole = str(lines[0])
  	quantity = str(lines[1])

  	res = requests.get(f"https://finance.yahoo.com/quote/{symbole}")
  	res.text
  	soup = bs4.BeautifulSoup(res.text,"lxml")
  	
  	test = soup.find('div',{'class': 'D(ib) Va(m) Maw(65%) Ov(h)'})

  	if test != None:
  		print("Position: " + symbole )
  		#print(symbole)
  		print("Number of shares: " + quantity)
  		print("Current change: " + soup.find('div',{'class': 'D(ib) Va(m) Maw(65%) Ov(h)'}).find_all('span')[0].text)
  		change = float(soup.find('div',{'class': 'D(ib) Va(m) Maw(65%) Ov(h)'}).find_all('span')[0].text)
  		position_change = int(quantity)*change
  		#print(position_change)
  		portfolio_change = portfolio_change + position_change
  		print('\n')

print("Total Change: " + str(portfolio_change))
  		
if int(portfolio_change) < 0:
	print("Losing money today")
	ser.write(b'a')
	sleep(1)
	read = ser.readline()
	print(read)

elif portfolio_change > 0:
	print("Making money today")
	ser.write(b'b')
	sleep(1)
	read = ser.readline()
	print(read)
else:
	print("No change")
	ser.write(b'c')
	sleep(1)
	read = ser.readline()
	print(read)

ser.close()