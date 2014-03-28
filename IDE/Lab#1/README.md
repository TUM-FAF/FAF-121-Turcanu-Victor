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

#### Connect to a remote server via SSH ####

I tried to connect to the server in modes: first on a Unix based OS (Ubuntu) and the second on Windows (using shell). It wasn't hard on neither of them, but you have to install the shell on Windows. To connect to a remote server via ssh I used the `ssh username@host:port`, where `username` was given and the `host` was the IP address of the remote server.

#### Initialize a repository on server, make a file, write your name in it and commit it ####

In order to perform the given task I followed the next steps:
	* `mkdir Turcanu_Victor_FAF_121` 


