import os
import openai
import random
openai.api_key = "sk-VuNQMum16VixnNQiGSOYT"+"3BlbkFJYSicKqLyQYSHGO149fy0"
f1 = open("noname_database.txt","a")


text0 = "a thread"
text1 = "one deep comment"
text2 = "two deep comment"
text3 = "three deep comment"
#default texts
rc = 3
#rc is maximum comments that can extend from a successfull depth check

prompt = []
f1.write("\n")
#formatting line
for i in range(0,len(prompt)):
    text0 = openai.Completion.create(
        model="text-davinci-003",
        prompt= "create a reddit post with the prompt:"+prompt[i]+"and do not include title markers, body markers, placeholders and the name reddit",
        max_tokens=500,
        temperature=1
    )
    f1.write("2 1~"+text0["choices"][0]["text"].replace("\n","")+"~davinciGPT3~0~\n") 
    count = random.randint(1,rc)
    for j in range(1,count+1):
        text1 = openai.Completion.create(
            model="text-davinci-003",
            prompt= "create a comment to the post with text: "+text0["choices"][0]["text"]+" and do not include placeholders or names",
            max_tokens=100,
            temperature=1
        )

        f1.write("3 1~"+text1["choices"][0]["text"].replace("\n","")+"~davinciGPT3~0~\n")
        dcheck = random.randint(0,3)
        #dcheck is depth check
        if dcheck > 0:
            count = random.randint(1,rc)
            for k in range(1,count+1):
                text2 = openai.Completion.create(
                    model="text-davinci-003",
                    prompt= "create a comment to the post with text: "+text1["choices"][0]["text"]+" and do not include placeholders or names",
                    max_tokens=100,
                    temperature=1
                )
                f1.write("4 1~"+text2["choices"][0]["text"].replace("\n","")+"~davinciGPT3~0~\n")
                dcheck = random.randint(0,2)
                if dcheck > 0:
                    count = random.randint(1,rc)
                    for l in range(0,count+1):
                        text3 = openai.Completion.create(
                            model="text-davinci-003",
                            prompt= "create a comment to the post with text: "+text2["choices"][0]["text"]+" and do not include placeholders or names",
                            max_tokens=100,
                            temperature=1
                        )
                        f1.write("5 1~"+text3["choices"][0]["text"].replace("\n","")+"~davinciGPT3~0~\n")
