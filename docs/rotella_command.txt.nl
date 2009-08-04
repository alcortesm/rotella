  1	ROTELLA(1)                      Commands Manual                     ROTELLA(1)
  2	
  3	NAME
  4	     rotella - A command line ROtella servent
  5	
  6	SYNOPSIS
  7	     rotella
  8	
  9	DESCRIPTION
 10	     rotella is a high-performance ROtella servent supporting v0.1 of the
 11	     ROtella protocol.
 12	
 13	     rotella is referred to as a servent - it is both client and server to the
 14	     network, allowing the user to both serve files to other ROtella network
 15	     members and to receive files from those members.
 16	
 17	     The terminal interface provided on the command line allows the user to
 18	     operate on information displayed.  Many commands will provide a numbered
 19	     list in its output; the command line interface remembers which numbers
 20	     were assigned during the last listing of the output type in question,
 21	     whether that be the list of connections, the list of searches, or the
 22	     last displayed list of results.  Rerunning the command that generated the
 23	     list will change the numbers which the user must use in following com-
 24	     mands to the newly generated numbers.
 25	     
 26	     When rotella starts, it obtains from the configuration file the URL for
 27	     a ROWebCache server. Then, using the ROWebCache protocol, rotella
 28	     obtains from the ROWebCache server a list of ROtella nodes. Finally,
 29	     a prompt is presented to the user. The user can invoke commands for
 30	     searching files in the ROtella network (find), for downloading a file
 31	     from any host in the network (get), for listing the files currently
 32	     being downloaded (info), for interrupting the download of any file
 33	     (stop), or for exiting rotella (exit).
 34	
 35	COMMANDS
 36	     find keyword
 37	       Searches for files in the network containing "keyword" in their names.
 38	       Once a search has located results, those results are listed, numbered 
 39	       with a result ID, with this format:
 40	         result ID) file name
 41	                    size
 42	                    IP:port
 43	       Example:
 44	         > find World
 45	         1) Eric Clapton-If I could Change The World.mp3
 46		    3.6M
 47	            69.244.21.191:6351
 48	         2) Gary Jules- Mad World.mp3
 49		    7.23M
 50	            70.178.17.75:6346
 51	         >
 52	
 53	       Once the user has viewed the results, she may use the result ID
 54	       provided in that list to perform a get, and attempt to download that
 55	       file.
 56	
 57	     get ID
 58	       Initiates download of the file defined by specific result IDs taken
 59	       from the last find command.
 60	
 61	       Example:
 62	         > get 2
 63	
 64	       Once download is started, rotella will try to get the requested file
 65	       from any hosts providing that file. The download will complete only
 66	       when the transfer is successful or stopped with the stop transfer
 67	       command.
 68	
 69	       The progress of downloads can be examined with the info command.
 70	
 71	     info [options ...]  
 72	       Displays information regarding current status of the client and 
 73	       its activities.  If executed without arguments, it will list 
 74	       information from all sections, as if the user had supplied all 
 75	       options to the command.  Using options will display only the 
 76	       requested information.  Multiple options may be provided.  These
 77	       options are
 78	
 79	       uploads
 80	         Displays each upload, or file sent to another ROtella user.
 81	         Each upload lists the host and port which the file is being
 82	         sent to, and various transfer statistics about the file
 83	         transfer in progress.  Each transfer is numbered, and can be
 84	         stopped using the stop command.
 85	
 86	       downloads
 87	         Displays all currently active downloads.  For each download,
 88	         information concerning the remote host selected for download-
 89	         ing and transfer statistics are provided. Each transfer is
 90	         numbered, and can be stopped using the stop command.
 91	
 92	       hosts
 93	         Displays the current content of the hosts cache, the top of the
 94	         iceberg of our 'known universe'.  Hosts in this list may be used to
 95	         create additional/future ROtella-net connections.  When the client
 96	         is in operational state, the host cache contains at least a few
 97	         dozen entries; normally not more than 100.
 98	
 99	       Example:
100	         > info uploads downloads
101		 Uploads:
102		   1) Eric Clapton-If I could Change The World.mp3
103		      62.195.18.49:6346
104		      4.5M/5.16M 
105		 Downloads:
106		   2) Gary Jules - Mad World.mp3
107		      86.10.142.237
108		      3.25K/2.86M 
109		   3) Graham Colton Band - All the World Tonight.mp3
110		      24.195.172.141
111		      0/5.45M  
112	         > info hosts
113		   68.52.28.249:36661
114		   24.195.172.141:6348
115		   86.10.142.237:6346
116		   62.195.18.49:6348
117		 >
118	
119	     stop ID
120	       Stops the file transfers corresponding to one transfer (upload or
121	       download) ID.  Note that this command does not remove partial
122	       files from the download directory.
123	
124	     exit
125	       Exits the servent.
126	
127	     set [variable] [value]
128	       This command is used to access the application's many options and pref-
129	       erences. Using the command without any parameters provides an organised
130	       list of all options available and their current values.  When the com-
131	       mand is called with a single variable name, but without a value, it
132	       will display the current settings of that variable.  To set a vari-
133	       able's contents, supply the value as the second parameter to the com-
134	       mand.
135	
136	       The variables supported are:
137	
138	       rowebcache
139	         URL where ROtella Web Caching System starts the initial connection.
140	
141	       localport
142	         Port used by the application (default 6346).
143	
144	       sharepath
145		  Path where shared files are located.
146	
147	       incomingpath
148	          Path where files being downloaded are stored. When the download is 
149	          completed the file is moved to the download path directory.
150	
151	       downloadpath
152	          Path where downloaded files will be stored.
153	
154	FILES
155	     When rotella starts, if a file named rotella.cfg exists in the current
156	     directory, it executes the set commands in it as if they were typed in
157	     the command prompt.
158	
159	     Example rotella.cfg file:
160	       set rowebcache http://www.somehost.com/path/to/script
161	       set localport 6346
162	       set sharepath /home/user/rotella/share/
163	       set incomingpath /home/user/rotella/incoming/
164	       set downloadpath /home/user/rotella/download/
165	
166	VERSION
167	     This manual documents rotella 0.1
168	
169	AUTHORS
170	     Celeste Campo, based on mutella, from:
171	
172	       Max Zaitsev   Original author and maintainer.
173	       Gregory R. Block
174	                     Co-maintainer and co-developer, build system, and
175			     portability changes
176	       Other Contributors
177	         Marco Herrn Original author of this manual page.
178	         Greg Parker Contribution of a one-shot replacment for poll() for
179		             systems only supporting select()
180	
181	SEE ALSO
182	      The specifications of the ROtella and the ROWebCache protocols.
183	
184	                                    Oct, 2005
