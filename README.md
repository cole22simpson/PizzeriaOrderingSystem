# Pizzeria Ordering System

![deli-food-delivery](https://github.com/user-attachments/assets/5c028453-6856-4bb9-b71e-a6b785a507dc)

## Description

This application keeps track of orders that a pizzeria receives and hands them off to various delivery services. All orders go through a buffer that is used to keep track of available slots for the food items. In this particular setup, the pizzeria is trying to promote the pizzas more than the sandwiches so they have put a limit on the number of sandwich orders that can be in the buffer at any given time. The number of active orders at one time is also limited so that the pizzeria is not taking on more than they can handle at once. There are multiple delivery services waiting to accept and transport the orders and all transactions are logged for proof of all completed orders.

## Usage

Once the repository is downloaded, the program can be run in the terminal with the following executable and arguments:
<br>
./fooddelivery -s 3 -p 9 -a 37 -b 40

-s: Number of milliseconds it takes for a sandwich order to be added to the system
<br>
-p: Number of milliseconds it takes for a pizza order to be added to the system
<br>
-a: Number of milliseconds it takes for Delivery Service A to acquire an order to be transported
<br>
-b: Number of milliseconds it takes for Delivery Service B to acquire an order to be transported
<br>
The max number of active orders and any limit on a particular item must be changed in the code itself.

## Questions

Message me on GitHub [here](github.com/cole22simpson)
<br>
Email me at cole22simpson@gmail.com
