
import random

user_win = 0
computer_wins = 0
options = ["rock","paper","scissor"]

while True:
    user_input = input("Type rock/paper/scissor or q to quit:").lower().strip()
    if user_input == "q":
       break    
  
    if user_input not in options :
        continue  

    random_number = random.randint(0,2)
    computer_guess = options[random_number]
    print("computer picked",computer_guess + ".")

    if user_input == "rock" and computer_guess == "scissor":
        print("you won!")
        user_win += 1

    elif user_input == "paper" and computer_guess == "rock":
         print("you won!")
         user_win += 1

    elif user_input == "scissor" and computer_guess == "paper":
        print("you won!")
        user_win += 1

    elif user_input == computer_guess:
        print("its a tie")

    else: 
         print("You lost!")
         computer_wins += 1

print("you won",user_win,"times.")
print("computer won",computer_wins,"times.")
  




    

