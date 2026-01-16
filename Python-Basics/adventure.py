name = input("What is your name:").strip()
print("Welcome",name,"to this adventure")
answer = input("you have come to a deadend you can either go left or right which way do you want to go? ").strip()

if answer == "left" or answer == "Left":
    answer = input("You have come to a river. You can either walk around it or swim across it. Type walk or swim: ").lower().strip()
    if answer == "swim":
        print("you swam across and got eaten by an alligator")
       
    elif answer == "walk":
        print("You walked for many miles and ran out of water and died thus losing the game")
       
    else:
        print("not a valid option.You lose")
    

elif answer == "right" or answer == "Right":
    answer = input("You have encountered a wobbly bridge do you want to cross it or head back. Type cross or head back:").lower().strip()
     
    if answer == "head back":
        print("you head back and lost")
       
    elif answer == "cross":
        answer = input("you just met a stranger do you want to talk yes or no: ").lower().strip()
        if answer == "yes":
            print("you just got gold from the stranger you WON!") 
        
        elif answer == "no":
            print("you reached another dead end")

        else:
            print("invalid input.You lose")
       
    else:
        print("not a valid option.You lose")

    
    

else:
    print("not a valid option.You lose")
    

    


