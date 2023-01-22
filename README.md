
# CFS - Computer Science Forums

## Data Type used

The website consists of so-called "Threads", which describe an object with [content, author, score, and sub-threads], as well as time-of-creation. 

Length of the content is bound by 1024 characters (1 kilobyte), and length of authors name is bound by 32 characters

### Form of data

With the existence of a root thread that serves as an anchor point, the entire data is in tree form with a variable number of children. 

These children are not ordered and are stored in a stack. To do this, a simple linked list of nodes is used, where each node stores 1 thread.

### Thread details

Threads have different "type", which is used only for display on the frontend part. Comment, Post, and Folder are those types. There's no difference between them in implementation.

The sub threads are not allocated at initialization but are added 1-by-1 dynamically through the add_subthread () function.

### Saving and Loading database

The website stores all of its posts in a `.txt` file, treating it as a database. There are empty lines at the beginning and the end of the file, which are necessary for operation.

Each thread occupies 1 line and is stored in the form
 `"{depth} {type}~{content}~{author}~{epoch}"`, 
where *type* and *epoch* are numbers representing the enumerated type of the Thread and epoch time in seconds for when the post was created respectively.

*Content* and *author* are strings that represent the entire content of the post - whether its the name of the folder or several sentences from the post and author is the nickname that the user chooses upon creation of the post.

*Depth* is a relative number that is used to indicate sub threads. `{depth+1}` is a sub thread of the latest `{depth}`, making it a pre-order traversal.

`{depth=0}` represents the root, and there can be only 1 entry with a depth of 0.


### Backend Communication

The communication with Python is done through sockets.

Upon receiving a command from Python, we first determine what command is it

There are: 
 - "list {PATH}"
 - "view {PATH}"
 - "post {CONTENT}"

where {PATH} is location of a Thread in a tree, i.e. CMPT/125 represents folder '125' located inside folder 'CMPT' which is directly under the root.

 - "list {PATH}" takes the path of a Thread and returns all subthreads that belong to it in a string where each line represents a thread.
For Posts and Comments it is in form: `"{type}~{author}~{epoch}"`
And for Folders it is in form: `"{type}~{content}~{author}~{epoch}"`, where content means folder's name
Later the python brings every string to the form of the Folder with '' (empty string) representing the content of Posts and Comments (since it's not provided)
If there are no subthreads - returns "failure".
Primarily used to display contents of the Folder or comments to the Post.

 - "view {PATH}" takes the path of a Thread and returns its content as a string.
Used for showing the entire content of the post. Allows to avoid rendering it unless the user actually needs it.

 - "post {CONTENT}" takes a string of content and creates a post with it, returning "success" if post was created and "failure" otherwise.

### Middle Stage

After receiving data from program on C, Python prepares the data to send it to JavaScript through Fetch API. 

The Flask server is able to translate the URL to a thread-path that correlates to an individual post.

From there, the Flask server communicates with the C Socket to learn about that thread or post.


## AI Post Generator

CSF uses OpenAI's davinciGPT3 to populate the site with posts that are almost indistinguishable from a real person's. 

Given some basic prompts davinciGPT3 can create whole threads and fill it with comments. 

`bulkcreate.py` allows you to generate as many threads as you want, as well as with include a random amount of comments, given a prompt. 

`proceduralgen.py`, when run, periodically comments on posts to emulate an active site, taking into account the contexts of the post
