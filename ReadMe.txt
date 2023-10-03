Extra Credit Readme: 

Building and Running Instructions. 
1. If not compiled run in the terminal: 
"g++ -o client client.cpp -std=c++17 -pthread"
and
"g++ -o server server.cpp -std=c++17 -pthread" 
2. In the terminal run: "./server" to start the server
3. Run "./client" in the terminal and input a user name
4. Repeat step 3 until you reach the desired number of users. 
5. Send messages by "U"USER":"MESSAGE""

Implementation: 
This was the culmination of programing assignment 1 and 2. Many of the ideas and concepts were used to make this chat client work correctly. 
Firstly lets dive into the client.cpp file. 

client.cpp: 

This file was simplier than server.cpp and followed a similer structure to the other programing assignments. The recieve_message function was a majority of this file and handled recieving messages from the server.
It allows users to send messages while recieving by following the logic in the main loop. The main loop sets up the sockets which was similar to other projects but getting the recieve message to work after creating usernames was tricky.

server.cpp: 
This file was the hardest part of this project. Much of the error handling and message handling was done here. Much of the logic here took much time to figure out but 
the handle_client function is completed. this function makes sure the user exist and determines what kind of message it is. In main we use this function to map through
the list of messages and display them to the user. 

Summary: 
This project was a great learning experience and the whole class coming together in this project was so cool. This project built on the ideas of the other two 
and made sense and was rewarding. Coming out of this project I am confident in my ability to implement basic sockets and connections between clients which
I was not expecting coming into this course. Thank you so much for everything Prof and TAs!!!
