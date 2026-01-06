import random

questions = {

"What is Python?" : "programming language",
"What symbol is used to write comments in Python?" : "#",
"What is a variable?" : "storage",
"Which data type stores whole numbers in Python?" : "int",
"What does print() do?" : "output",
"What is a list?" : "collection",
"How do you find the length of a list x?" : "len",
"Which keyword defines a function?" : "def",
"What does len() return?" : "count",
"What is a loop used for?" : "repetition",
"Which loop is used when repetitions are known?" : "for",
"What does range(5) end at?" : "4",
"What does an if statement do?" : "condition",
"What is indentation used for?" : "blocks",
"What does break do?" : "stop",
"What is a docstring?" : "documentation",
"What does \\n represent?" : "newline",
"How is a matrix represented in Python?" : "list",
"What does zip(*matrix) do?" : "transpose",
"What error occurs with wrong indentation?" : "IndentationError",
}

def python_trivia_game():
    questions_list = list(questions.keys())                                                  
    score = 0                                                                                
    total_questions = 5                                                                     
    selected_questions = random.sample(questions_list,total_questions)                       
    for idx, question in enumerate(selected_questions,start = 1):
        print(idx,question)
        answer = input().strip().lower()
        correct_answer = questions[question].lower()
        
        if answer == correct_answer:
            print("okie dokie!")
            score += 1

            
        elif answer != correct_answer:
            print("wrong!")
            

        else:
            if answer =="q":
                quit()    

    print("You answered",score,"questions correctly")

python_trivia_game()

    


