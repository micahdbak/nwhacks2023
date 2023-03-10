import os
import openai
import backend
import time
import random
openai.api_key = "sk-VuNQMum16VixnNQiGSOYT"+"3BlbkFJYSicKqLyQYSHGO149fy0"

content = openai.Completion.create(
    model="text-davinci-003",
    prompt= "create a reddit post with the prompt:\"I am doing a hackathon next week, how should I prepare?\"and do not include title markers, body markers, placeholders and the name reddit",
    max_tokens=500,
    temperature=1
)
time.sleep(5)
response = backend.transact('post CMPT davinciGPT3').decode()
if response != 'failure':
    backend.transact(content["choices"][0]["text"]).decode()
    CMPTfull = backend.transact('list CMPT').decode()
    contentidindex = CMPTfull.split('\n')
    #split
    # x x x
    # x x x 
    # x x x
    #into lists
    contentidsplit = (contentidindex[0]).split('~') #split x x x into a list
    contentid = contentidsplit[2] #grabs 3rd element which is epoch of post
    contentcontent = backend.transact('view CMPT/'+contentid).decode() #grabs content of post
    while True:
        allcomments =  backend.transact('list CMPT/'+contentid).decode() #allcomments is the string of all comments in a post
        if 'ElvinZukic' in allcomments: #check for non-GPT user
            allcommentsindex = allcomments.split('\n') #breaks it into a list of lines
            for i in (allcommentsindex):
                if 'ElvinZukic' in allcomments[i]: #finds what line it is in
                    break
            allcommentssplit = (allcommentsindex[i].split('~')) #breaks that line into another list
            commentcontent = backend.transact('view CMPT/'+contentid+'/'+allcommentssplit[2]).decode() #uses epoch of the comment (indx 2) to view content 
            response = backend.transact('post CMPT/'+contentid+'/'+commentcontent+' davinciGPT3').decode() #prepares to post it
            if response != 'failure': #check if its still there
                comment = openai.Completion.create( #generate text
                    model="text-davinci-003",
                    prompt= "create a comment to the post with text: "+commentcontent+" and do not include placeholders or names ",
                    max_tokens=200,
                    temperature=1
                )
                backend.transact(comment["choices"][0]["text"]).decode() #post it
        else:
            time.sleep(7)
            response = backend.transact('post CMPT/'+contentid+' davinciGPT3').decode() #post a comment to the main post every 7 seconds
            if response != 'failure':
                comment = openai.Completion.create(
                    model="text-davinci-003",
                    prompt= "create a comment to the post with text: "+contentcontent+" and do not include placeholders or names ",
                    max_tokens=200,
                    temperature=1
                )
                backend.transact(comment["choices"][0]["text"]).decode()
