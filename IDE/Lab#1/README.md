IDE Laboratory Work #1 
===========================

Subject
-------------------------

Command Line Interface; CLI Editors; Setting Server Environment; Version Control Systems

Objectives
--------------------------

* Understanding and using CLI (basic level)
* Administration of a remote linux machine using SSH
* Ability to work remotely (remote code editing)
* Ability to work with VCS

Executed Tasks
------------------

* Mandatory tasks
	- Connect to a remote server via SSH
	- Initialize a repository on server
	- Create a file in repository folder, write in your name, save it and commit it
* Objective with points
	- Connect to server using public key (1 pt)
	- Create 2 more branches with at least one unique committed file per branch (1 pt)
	- Set a branch to track a remote origin on which you are able to push (ex. github, bitbucket or a custom server) (1 pt)
	- Reset a branch to previous commit, reset a branch to some specific commit (1 pt)
	- Restore a reset branch back to its previous state (1 pt)
	- GIT cherry-pick, rebase (1 pt)
	- Create a VCS hook (1 pt)
	- Make your CLI text editor to highlight code (1 pt)
	- Create a VCS alias (1 pt)
	- Master any CLI editor (ex. VIM). Learn 10 commands' sets (a/A/i/I/o/O is one set) to prove your mastery (1 pt)
	- Create your own server (ex. virtual machine) (2 pt)
* Additional tasks
	- Referencing to commits using branch titles


##Theoretical Work:##

There are a lot of benefits of working on a remote server via ssh. First of all - using ssh keys it's more secure than simple passwords. Secondly - you have a lot of power over file management and the tasks you can do than using a GUI. Working through a command-line interface it is more useful than through a GUI because is the secodn situation there are a lot of hidden backtracks that you can't use and this creates disconfort. 

I used Vim text editor for editig the text file as well as the code. It has a lot of advantages but also and a lot of disadvantages. The bigest advantage is that it doesn't need a lot of resources in order to run. The disadvantage is the antiintuitive working process. Sometimes it is unpleasant to manage with data, but in the same manner it is fast.

And finally, but not the least important, is the Git VCS. It a very powerful tool wich allows the users to keep the track of their work and data. It diminishes the risk of losing data and offers a great opportunity for users to reuse their data comming back to previous stages of work.


##Practical Work:##

### Connect to a remote server via SSH ###

I tried to connect to the server in modes: first on a Unix based OS (Ubuntu) and the second on Windows (using shell). It wasn't hard on neither of them, but you have to install the shell on Windows. To connect to a remote server via ssh I used the `ssh username@host:port`, where `username` was given and the `host` was the IP address of the remote server.

### Initialize a repository on server, make a file, write your name in it and commit it ###

In order to perform the given task I followed the next steps:

* `mkdir Turcanu_Victor_FAF_121`, `cd Turcanu_Victor_FAF_121` - create a new directory and enter it
* `mkdir git`, `cd git`, `git init` - create a new directory git, access it and initialize an empty repository
* `touch file.txt`, `vim file.txt`, `:i`, `This is my fist file using vim`, ^C, `:wq` - create a new file, access it using vim, write something, close it saving the changes
* `git add .`, `git commit -m "first commit"` - adding files and committing them

### Connect to server using public key ###
 I copyed the content of the file `id_rsa.pub` from the `.ssh` directory from my local machine to the file `authorized_keys` from the `.ssh` directory on the remote repository. I did it using `clip < .ssh/id_rsa.pub`.


 ### Working with Git ###

 * Creating new branches and files

``` sh
git branch branch1
git branch branch2
git checkout branch1
git touch file1
git add .
git commit -m "branch1: file1"
git checkout branch2
git touch file2
git add .
git commit -m "branch2: file2"

```

* Reset a branch to a previous commit, and back

``` sh

git reset --hard HEAD^ 	# resets the head of the tree to the previous commitment
git reflog
# list of commitments and their ids
git reset --hard <commit's id>	# here the commit's id is the id of the one before last commit

```
* Cherry-pick, rebase

``` sh

git reflog
git checkout master
git cherry-pick <commit id1>   # commit id1 is now applied to the master branch and commited (as a new commit) in master

git checkout -b branchx <commit id1>  # creates a new branch at what commit id1
git rebase --onto master <commit id2>   # the result is that commits through id1 to id2 are applied to master

```

* Creating a VCS alias (Git)

``` sh

git config --global alias.vic = "commit -m"

``` 

* Creating a VCS hook 

Here I encoutered a little problem on Windows (on Ubuntu it worked perfectly), becouse of the path setting, so I decided to make it on Unix anyway, listening to a piece of advice from my colegue Andrei Istratii, and made it on the Windows but using the shell and conecting to the remote server which was offered to us at the lab. 

``` sh

cd .git/hooks  # assuming that we are in the Turcanu_Victor_FAF_121/git directory
touch post-commit  #  create an empty file for hookin the post-commit event
vim post-commit  
:i
	#!/usr/bin/env python 

	print "Hello commit"

	^C
:wq
chmod +x post-commmit
cd ..
cd ..
touch random_file.txt
git add .
git commit -m "message"  #and after this appears the string "Hello commit"

```

### Create your own server ###

For this purpose I used Ubuntu and used the following commands:

``` sh

sudo apt-get install openssh-server 
sudo service ssh start
ifconfig   # here we get the IP for future connection

```

After this I connected from the stationary PC and accessed the home folder, created a file.txt and wrote "Hello! It's PC". After everything is done, the server is closed using

``` sh

sudo service ssh stop

```

### Master Vim editor ###

As the Vim editor highlights the code syntax by default, it was only a matter of finding the command which manages this: `:syntax on/off`.

- `:i` - Insert text before the current cursor position
- `:w` - Write file to disk
- `:q` - Quits the editor
- `: 12, 15 w aux` - Writes line 12 to 15 in  a file aux
- `:d` - Delets an entire line, even if it's empty
- `:p` - Pastes below the cursor
- `u` - Undu last change
- 'J' - Joins the end of the current line with the beginning of the next line
- `r` - Replace one character at the cursor position
- `.` - Repeat last command


Conclusion
---------------------

This laboratory work was very useful, even at the begining I didn't knew what does mean 70% of the tasksand how to do them. In the end they were pretty simple, the only thing need was just to search and find for each task and understand. Here helped a lod the documentation which I found on github and olso on other tutoril sites. 

Olso I understood that for a programmer working on Ubuntu is much easier. But in the same manner working on Windows have it's benefits because makes you search into the deep foundation of the things and not to look just from the surface at the automatic processes which occur on Ubuntu. Although you have more freedom on Ubuntu. 

The work with Git was very useful and made me understand some little tricks which will help me on my future project and as a good start of understanding a complex system of data management. I remembered the graph theory and its implementation which allows to manage a big amount of data and processes at the same time. 

I spent a great time doing this laboratory work :)