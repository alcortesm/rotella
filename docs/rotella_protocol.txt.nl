  1	Request for Comments: NNNN                                 Celeste Campo
  2	Category: Informational                                     October 2005
  3	
  4	
  5	
  6	                        ROtella 0.1
  7	
  8	
  9	Status of this Memo
 10	
 11	   This is a draft.
 12	
 13	
 14	Copyright Notice
 15	
 16	   This document describes a simplification of the Gnutella protocol
 17	   to be used as the specification for the practical assigment in the
 18	   Redes de Ordenadores II (Computer Netwoks II) course in the
 19	   Carlos III University of Madrid. For more details see 
 20	   http://www.it.uc3m.es/celeste/docencia/inf/rroo/
 21	   
 22	   For the original Gnutella protocol, copyright (C) 2002, Tor
 23	   Klingberg & Raphael Manfredi. All Rights Reserved.
 24	   Permission is granted to make verbatim copies of this document, 
 25	   provided the Copyright Notice is preserved.
 26	
 27	
 28	Table of Contents
 29	
 30	   1   Introduction
 31	   1.1   Purpose
 32	   1.2   Requirements
 33	   1.3   Terminology
 34	   2   Protocol Definition
 35	   2.1   Initiating a Connection
 36	   2.2   ROtella Messages
 37	   2.2.1   Message Header
 38	   2.2.2   Query (0x80)
 39	   2.2.3   Query Hit (0x81)
 40	   2.2.4   Use of Query and Query Hit
 41	   2.2.4.1   Forwarding and routing of Query and Query Hit messages
 42	   2.2.4.2   When and how to send new Query messages.
 43	   2.2.4.3   When and how to respond with Query Hit messages.
 44	   2.2.5   Bye (0x02)
 45	   3   Protocol Usage
 46	   3.1   Flow Control
 47	   4   File Transfer
 48	   4.1   Normal File Transfer
 49	   4.2   Sharing
 50	   5   Credits
 51	
 52	
 53	1 Introduction
 54	
 55	1.1 Purpose     
 56	
 57	ROtella is a decentralized peer-to-peer system. It allows the
 58	participants to share resources from their system for others to
 59	see and get, and locate resources shared by others on the network.
 60	
 61	Each participant launches a ROtella program, which will seek out 
 62	other ROtella nodes to connect to.  This set of connected nodes 
 63	carries the ROtella traffic, which is essentially made of queries, 
 64	replies to those queries, and also other control messages to 
 65	facilitate the discovery of other nodes.
 66	
 67	Resource data exchanges between nodes are negotiated using the 
 68	standard HTTP protocol.  The ROtella network is only used to locate 
 69	the nodes sharing those resources.
 70	
 71	1.2 Requirements
 72	
 73	The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT",
 74	"SHOULD", "SHOULD NOT", "RECOMMENDED", "MAY", and "OPTIONAL" in this
 75	document are to be interpreted as described in RFC 2119 [34].
 76	
 77	
 78	1.3 Terminology
 79	
 80	Servent         A program participating in the ROtella network is 
 81	                called a servent. The words "peer", "node" and "host"
 82	                have similar meanings, but refers to a network 
 83	                participant rather than a program. When a servent 
 84	                have a clear client or server role the words "client"
 85	                or "server" may be used. The word "client" is 
 86	                sometimes used as a synonym for servent. This is a 
 87	                contraction of "SERVer" and "cliENT", Some other 
 88	                documents use the word "servant" instead of servent.
 89	
 90	Message         Messages are the entity in which information is
 91	                transmitted over the network. Sometimes the word 
 92	                "packet" is used with the same meaning. Some other 
 93	                documents use the word "descriptor"
 94	
 95	GUID            Globally Unique IDentifier.  This is a 16-byte long
 96	                value made of random bytes, whose purpose it is to
 97	                identify servents and messages.  This identification
 98	                is not a signature, just a way to identify network
 99	                entities in a unique manner.
100	
101	
102	2 Protocol Definition
103	
104	The ROtella protocol defines the way in which servents communicate 
105	over the network. It consists of a set of messages used for 
106	communicating data between servents and a set of rules governing 
107	the inter-servent exchange of messages. Currently, the following 
108	messages are defined:
109	
110	Query           The primary mechanism for searching the distributed 
111	                network. A servent receiving a Query message will
112	                respond with a Query Hit if a match is found against 
113	                its local data set.
114	
115	QueryHit        The response to a Query. This message provides the
116	                recipient with enough information to acquire the data
117	                matching the corresponding Query.
118	
119	Bye             An optional message used to inform the remote host 
120	                that you are closing the connection, and your reason 
121	                for doing so.
122	
123	
124	2.1 Initiating a Connection
125	
126	A ROtella servent connects itself to the network by establishing a 
127	connection with another servent currently on the network. 
128	
129	Techniques for finding the first host are described in ROtella web
130	caching specification. 
131	
132	Once the first connection is established, the addresses of more hosts
133	will be supplied over the network. The default ROtella port is 6346,
134	but servents MAY use any unused port.
135	
136	Once the address of another servent on the network is obtained, a 
137	TCP/IP connection to the servent is created, and a handshaking 
138	sequence is initiated. The client is the host initiating the 
139	connection and the server is the host receiving it. "<cr>" refers 
140	to ASCII character 13 (carriage return), and "<lf>" to 10 (new line).
141	
142	   1. The client establishes a TCP connection with the server.
143	   2. The client sends "ROTELLA CONNECT/0.1<cr><lf>".
144	   3. The client sends headers each terminated by "<cr><lf>", with
145	      an extra "<cr><lf>" at the end.
146	   4. The server responds with "ROTELLA/0.1 200 <string><cr><lf>".
147	      <string> SHOULD be "OK", but servents SHOULD just look for the
148	      "200" code.
149	   5. The server sends all its headers, in the same format as in (3).
150	   6. The client sends "ROTELLA/0.1 200 OK<cr><lf>, as in (4) if
151	      after parsing the server's headers, it still wishes to connect.
152	      Otherwise, it needs to reply with an error code and close the
153	      connection.
154	   7. Both client and server send binary messages at will.  
155	
156	Headers follow the standards described in RFC822 and RFC2616.  Each
157	header is made of a field name, followed by a colon, and then the 
158	value.  Each line ends with the <cr><lf> sequence, and the end of the
159	headers is marked by a single <cr><lf> line.  Each line normally 
160	starts a new header, unless it begins with a space or an horizontal 
161	tab (ASCII codes 32 and 9 in decimal, respectively), in which case it
162	continues the preceding header line.  The extra spaces and tabs may 
163	be collapsed into a single space as far as the header value goes.  
164	For instance:
165	
166	    First-Field: this is the value of the first field<cr><lf>
167	    Second-Field: this is the value<cr><lf>
168	        of the<cr><lf>
169	        second field<cr><lf>
170	    <cr><lf>
171	
172	The header above is made of two fields, "First-Field" and "Second-
173	Field" whose values are respectively "this is the value of the first 
174	field" and "this is the value of the second field" (leading spaces of
175	the continuation were collapsed).  Note that the leading space 
176	between the ":" ending the field name and the start of the value 
177	string does not count.
178	
179	Multiple header lines with the same field name are identical to one
180	header line where all the values of the fields would be separated by 
181	",". This means:
182	
183	    Field: first<cr><lf>
184	    Field: second<cr><lf>
185	
186	is strictly equivalent to saying:
187	
188	    Field: first,second<cr><lf>
189	
190	In other words, order matters in that case.
191	
192	Here is a sample interaction between a client and a server.  Data 
193	sent from client to server is shown on the left; data sent from 
194	server to client is shown on the right.
195	
196	     Client                           Server
197	     -----------------------------------------------------------
198	     ROTELLA CONNECT/0.1<cr><lf>
199	     User-Agent: BearShare/1.0<cr><lf>
200	     <cr><lf>
201	                                      ROTELLA/0.1 200 OK<cr><lf>
202	                                      User-Agent: BearShare/1.0<cr><lf>
203	                                      <cr><lf>
204	     ROTELLA/0.1 200 OK<cr><lf>
205	     <cr><lf>
206	
207	     [binary messages]                [binary messages]
208	
209	A few notes about the responses: first, the client (server) SHOULD
210	disconnect if receiving any response other than "200" at step 4
211	(6).  There is no need to define these error codes now.  Second,
212	servents SHOULD ignore higher version numbers in steps (2), (4), and
213	(6).  For example, it is perfectly legal for a future client to
214	connect to a server and send "ROTELLA CONNECT/0.2".  The server
215	SHOULD respond with "ROTELLA/0.2 200 OK" if it supports the 0.2
216	protocol, or "ROTELLA/0.1 200 OK" otherwise.
217	
218	A few notes about the headers: servents SHOULD use standard HTTP
219	headers whenever appropriate. For example, servents MUST use the
220	standard "User-Agent" header. Headers unknown to the servent MUST be
221	ignored.
222	
223	A connection MUST be rejected when there are currently three servents
224	already connected.
225	
226	When rejecting a connection, a servent MUST, if possible, provide 
227	the remote host with a list of other ROtella hosts, so it can try 
228	connecting to them. This MUST be done using the X-Try header.
229	
230	An X-Try header can look like:
231	
232	        X-Try:1.2.3.4:1234,3.4.5.6:3456
233	
234	There MAY be a space after the colon and after each comma. There MAY 
235	be multiple X-Try headers in one header set. The header MAY end with
236	an extra comma.  The header MAY be formatted on several lines using
237	continuations.
238	
239	Each item in the X-Try header gives the IP address of a servent
240	and its listening port number.
241	
242	The normal status code for rejecting a connection because the servent
243	is busy is "503 " followed by "Busy" or another description string.
244	
245	
246	2.2 ROtella Messages
247	
248	Once a servent has connected successfully to the network, it 
249	communicates with other servents by sending and receiving ROtella 
250	protocol messages. Each message is preceded by a Message Header with 
251	the byte structure given below.
252	
253	Note 1: One ROtella message may be split up on multiple IP-packets. 
254	This means one can never assume a ROtella message ends when the chunk of 
255	data read from the socket ends.
256	
257	Note 2: All fields in the following structures are in little-endian 
258	byte order unless otherwise specified.
259	
260	Note 3: All IP addresses in the following structures are in IPv4 
261	format. For example, the IPv4 byte array
262	
263	    0xD0     0x11     0x32     0x04
264	    byte 0   byte 1   byte 2   byte 3
265	
266	represents the dotted address 208.17.50.4.
267	
268	
269	2.2.1 Message Header
270	
271	The message header is 23 bytes divided into the following fields.
272	
273	    Bytes:  Description:
274	    0-15    Message ID/GUID (Globally Unique ID)
275	    16      Payload Type
276	    17      TTL (Time To Live)
277	    18      Hops
278	    19-22   Payload Length
279	
280	Message ID      A 16-byte string (GUID) uniquely identifying the
281	                message on the network. 
282	                        
283	                Servents SHOULD store all 1's (0xff) in byte 8 of the
284	                GUID.  (Bytes are numbered 0-15, inclusive.) This 
285	                serves to tag the GUID as being from a modern 
286	                servent.
287	        
288	                Servents SHOULD initially store all 0's in byte 15 of
289	                the GUID. This is reserved for future use.
290	
291	                The other bytes SHOULD have random values.
292	
293	Payload         Indicates the type of message
294	Type            0x80 = Query
295	                0x81 = Query Hit
296	                0x02 = Bye		
297	
298	TTL             Time To Live. The number of times the message 
299	                will be forwarded by ROtella servents before it is 
300	                removed from the network. Each servent will decrement
301	                the TTL before passing it on to another servent. When
302	                the TTL reaches 0, the message will no longer be 
303	                forwarded (and MUST not).
304	
305	Hops            The number of times the message has been forwarded.
306	                As a message is passed from servent to servent, the
307	                TTL and Hops fields of the header must satisfy the 
308	                following condition:
309	                TTL(0) = TTL(i) + Hops(i)
310	                Where TTL(i) and Hops(i) are the value of the TTL and
311	                Hops fields of the message, and TTL(0) is maximum 
312	                number of hops a message will travel (usually 7).
313	
314	Payload         The length of the message immediately following 
315	Length          this header. The next message header is located 
316	                exactly this number of bytes from the end of this 
317	                header i.e. there are no gaps or pad bytes in the 
318	                ROtella data stream. Messages SHOULD NOT be larger
319	                than 4 kB.
320	
321	The Payload Length field is the only reliable way for a servent to 
322	find the beginning of the next message in the input stream. 
323	Therefore, servents SHOULD rigorously validate the Payload Length 
324	field for each message received.  If a servent becomes out of synch 
325	with its input stream, it SHOULD close the connection associated with
326	the stream since the upstream servent is either generating, or 
327	forwarding, invalid messages.
328	
329	Abuse of the TTL field in broadcasted messages (Query) will lead to 
330	an unnecessary amount of network traffic and poor network 
331	performance.  Therefore, servents SHOULD carefully check the TTL 
332	fields of received query messages and lower them as necessary.  
333	Assuming the servent's maximum admissible Query message life is 7 
334	hops, then if TTL + Hops > 7, TTL SHOULD be decreased so that TTL + 
335	Hops = 7.  Broadcasted messages with very high TTL values (>15) 
336	SHOULD be dropped.
337	
338	Immediately following the message header, is a payload consisting 
339	of one of the following messages.
340	
341	2.2.2 Query (0x80)
342	
343	Since Query messages are broadcasted to many nodes, the total size 
344	of the message SHOULD not be larger than 256 bytes. Servents MAY drop
345	Query messages larger that 256 bytes, and SHOULD drop Query messages 
346	larger than 4 kB.
347	
348	A Query message has the following fields:
349	
350	Bytes:  Description:
351	0-1     Minimum Speed. The minimum speed (in kb/second) of servents
352	        that should respond to this message. A servent receiving a 
353	        Query message with a Minimum Speed field of n kb/s SHOULD 
354	        only respond with a Query Hit if it is able to communicate at
355	        a speed >= n kb/s.
356	        
357	2-      Search Criteria. This field is terminated by a NUL (0x00).
358	
359	        See section 2.2.4.3 for rules and information on how to 
360	        interpret the Search Criteria
361	        
362	2.2.3 Query Hit (0x81)
363	
364	Query Hit messages has the following fields:
365	
366	Bytes:  Description:
367	0       Number of Hits. The number of query hits in the result set 
368	        (see below).
369	        
370	1-2     Port. The port number on which the responding host can accept
371	        incoming HTTP file requests. This is usually the same port as
372	        is used for ROtella network traffic, but any port MAY be 
373	        used.
374	        
375	3-6     IP Address. The IP address of the responding host. 
376	        Note: This field is in big-endian format.
377	
378	7-10    Speed The speed (in kb/second) of the responding host.
379	        
380	11-     Result Set. A set of responses to the corresponding Query. 
381	        This set contains Number_of_Hits elements, each with the 
382	        following structure:
383	        
384	        Bytes:  Description:
385	        0-3     File Size. The size (in bytes) of the file whose 
386	                index is File_Index.
387	
388	        4-      File Index. A string, assigned by the responding 
389	                host, which is used to uniquely identify the file 
390	                matching the corresponding query. Terminated by a
391	                null (i.e. 0x00).
392	     
393	        x-      File Name. The name of the file whose index is 
394	                File_Index. Terminated by a null (i.e. 0x00) 
395	
396	Last 16 Servent Identifier. A 16-byte string uniquely identifying the
397	        responding servent on the network. This SHOULD be constant 
398	        for all Query Hit messages emitted by a servent and is 
399	        typically some function of the servent's network address.
400	
401	2.2.4 Use of Query and Query Hit
402	
403	2.2.4.1 Forwarding and routing of Query and Query Hit messages 
404	
405	A servent SHOULD forward incoming Query messages to all of its 
406	directly connected servents, except the one that delivered the 
407	incoming Query.
408	
409	A servent MUST decrement a message header's TTL field, and 
410	increment its Hops field, before it forwards the message to any 
411	directly connected servent. If, after decrementing the header's TTL 
412	field, the TTL field is found to be zero, the message MUST NOT 
413	be forwarded along any connection.
414	
415	A servent receiving a message with the same Payload Message and 
416	Message ID as one it has received before, MUST discard the 
417	message. It means the message has already been seen.
418	
419	QueryHit messages MUST only be sent along the same path that 
420	carried the incoming Query message. This ensures that only those 
421	servents that routed the Query message will see the QueryHit 
422	message in response. A servent that receives a QueryHit message
423	with  Message ID = n, but has not seen a Query message with 
424	Message ID = n SHOULD remove the QueryHit message from the 
425	network.
426	
427	
428	2.2.4.2 When and how to send new Query messages.
429	
430	Query messages are usually sent when the user initiates a search. 
431	
432	The TTL value of a new query created by a servent SHOULD NOT be 
433	higher than 7, and MUST NOT be higher than 10. The hops value MUST be
434	set to 0.
435	
436	
437	2.2.4.3 When and how to respond with Query Hit messages.
438	
439	When a servent receives an incoming Query message it SHOULD match 
440	the Search Criteria of the query against its local shared files. 
441	
442	The Search Criteria is text, and it has never been specified which 
443	charset that text was encoded with. Therefore, servents MUST assume 
444	it is pure ASCII only.
445	
446	The Search Criteria is a keyword. A servent SHOULD only respond with 
447	files that has the keyword.
448	
449	Regular expressions are not supported and common regexp "meta-
450	characters" such as "*" or "." will either stand for themselves or be
451	ignored. The matching SHOULD be case sensitive.  Empty queries or 
452	queries containing only 1-letter words SHOULD be ignored.
453	
454	Query messages with TTL=1, hops=0 and Search Criteria="    " (four
455	spaces) are used to index all files a host is sharing. Servents 
456	SHOULD reply to such queries with all its shared files. Multiple 
457	Query Hit messages SHOULD be used if sharing many files. Allowed 
458	reasons not to respond to index queries include privacy and 
459	bandwidth. 
460	
461	Query Hit messages MUST have the same Message ID as the Query message
462	it is sent in reply to. The TTL SHOULD be set to at least the hops
463	value of the corresponding query plus 2, to allow the Query Hit to
464	take a longer route back, if necessary. The TTL value MUST be at
465	least the hops value of the corresponding query, and the initial
466	hops value of the Query Hit message MUST (as usual) be set to 0.
467	Some servents use a TTL of (2 * Query_TTL + 2) in their replies to
468	be sure that the reply will reach its destination.  Replies with
469	high TTL level SHOULD be allowed to pass through.
470	
471	2.2.9 Bye (0x02)
472	
473	The Bye message is an OPTIONAL message used to inform the 
474	servent you are connected to that you are closing the connection. 
475	
476	Servents supporting the Bye message MUST indicate that by sending 
477	the following header in the handshaking sequence:
478	
479	        Bye-Packet: 0.1
480	
481	Servents MUST NOT send Bye messages to hosts that has not indicated 
482	support using the above header.  
483	
484	A Bye packet MUST be sent with TTL=1 (to avoid accidental propagation
485	by an unaware servent), and hops=0 (of course).
486	
487	A servent receiving a Bye message MUST close its connection 
488	immediately. The servent that sent the packet MUST wait a few 
489	seconds for the remote host to close the connection before closing 
490	it.  Other data MUST NOT be sent after the Bye message.  Make sure 
491	any send queues are cleared. 
492	
493	The servent that sent by Bye message MAY also call shutdown() with 
494	'how' set to 1 after sending the Bye message, partially closing the 
495	connection.  Doing a full close() immediately after sending the Bye 
496	messages would prevent the remote host from possibly seeing the Bye 
497	message.
498	
499	After sending the Bye message, and during the "grace period" when
500	we don't immediately close the connection, the servent MUST read
501	all incoming messages, and drop them unless they are Query Hits, 
502	which MAY still be forwarded (it would be nice to the
503	network).  The connection will be closed as soon as the servent
504	gets an EOF condition when reading, or when the "grace period"
505	expires.
506	
507	A Bye message has the following fields:
508	Bytes:  Description:
509	
510	0-2     Code. The presence of the Code allows for automated processing
511	        of the message, and the regular SMTP classification of error 
512	        code ranges should apply. Of particular interests are the 
513	        200..299, 400..499 and 500..599 ranges.
514	
515	        The code is codified as a three ASCII character string.
516	
517	        Here is the general classification ("User" here refers to the 
518	        remote node that we are disconnecting from):
519	
520	        2xx     The User did nothing wrong, but the servent chose to 
521	                close the connection: it is either exiting normally 
522	                (200), or the local manager of the servent requested 
523	                an explicit close of the connection (201).
524	
525	        4xx     The User did something wrong, as far as the servent is
526	                concerned. It can send packets deemed too big (400), 
527	                too many duplicate messages (401), relay improper 
528	                queries (402), relay messages deemed excessively long-
529	                lived [hop+TTL > max] (403), send too many unknown 
530	                messages (404), the node reached its inactivity 
531	                timeout (405), it failed to reply to a ping with TTL=1
532	                (406), or it is not sharing enough (407).
533	
534	        5xx     The servent noticed an error, but it is an "internal" 
535	                one. It can be an I/O error or other bad error (500), 
536	                a protocol desynchronization (501), the send queue 
537	                became full (502).
538	
539	3       ASCII character blank (0x20).
540	
541	2-      NULL-terminated Description String. The format of the String 
542	        is the following ( refers to "\r" and  to "\n"):
543	
544	                Error message, as descriptive as possible
545	
546	
547	3 Protocol Usage
548	
549	Apart from the protocol definition in section 2, there are also 
550	some guidelines on how to use the protocol. These are not absolutely
551	necessary to participate in the network, but very important for an
552	effective network.
553	
554	
555	3.1 Flow Control
556	
557	It is very important that all servents have a system for regulating 
558	the data that passes through a connection.
559	
560	The most simple way is to close a connection if it gets overloaded.
561	A better way is to drop broadcasted packets to reduce the amount of
562	bandwidth used.  A much better way is to do the following:
563	
564	Implement an output queue, listing pending outgoing messages in
565	FIFO order.  As long as the queue is less than, say, 25% of its
566	max size (in bytes queued, not in amount of messages), do nothing.
567	If the queue gets filled above 50%, enter flow-control mode.  You
568	stay in flow-control mode (FC mode for short) as long as the queue
569	does not drop below 25%.  This is called "hysteresis".
570	
571	The queue size SHOULD be at least 150% of the maximum admissible
572	message size.
573	
574	In FC mode, all incoming queries on the connection are dropped.
575	The rationale is that we would not want to queue back potentially
576	large results for this connection since it has a throughput problem.
577	
578	Messages to be sent to the node (i.e. queued on the output queue)
579	are prioritized:
580	
581	* For broadcasted messages, the more hops the packet has traveled,
582	  the less prioritary it is.  Or the less hops, the more prioritary.
583	  This means your own queries are the most prioritary (hops = 0).
584	
585	* For replies (query hits), the more hops the packet has traveled,
586	  the more prioritary it is.  This is to maximize network usefulness.
587	  The packet was relayed by many hosts, so it should not be dropped
588	  or the bandwidth it used would become truly wasted.
589	
590	* Individual messages are prioritized thusly, from the most
591	  prioritary to the least: Query Hit, Query.
592	
593	Normally, all messages are accepted.  However, when the message to
594	enqueue would make the queue fill to more than 100% of its maximum
595	size, any queued message less prioritary in the queue is dropped.
596	If enough room could be made, enqueue the packet.  Otherwise, if the
597	message is a Query, drop it.  If not, send a Bye 502 (Send Queue
598	Overflow) message.
599	
600	4 File Transfer
601	
602	4.1 Normal File Transfer
603	
604	Once a servent receives a QueryHit message, it may initiate the 
605	direct download of one of the files described by the message's Result
606	Set. Files are downloaded out-of-network i.e. a direct connection 
607	between the source and target servent is established in order to 
608	perform the data transfer. File data is never transferred over the 
609	ROtella network.
610	
611	The file download protocol is HTTP 1.1.
612	
613	The servent initiating the download sends a request string on the 
614	following form to the target server:
615	
616	    GET /<File Index>/<File Name> HTTP/1.1<cr><lf>
617	    User-Agent: ROtella<cr><lf>
618	    Host: 123.123.123.123:6346<cr><lf>
619	    Connection: close<cr><lf>
620	    <cr><lf>
621	
622	where <File Index> and <File Name> are one of the File Index/File 
623	Name pairs from a QueryHit message's Result Set.
624	
625	"Connection: close" means that the connection MUST be closed after the
626	transfer. Servents MUST NOT attempt to download multiple files from
627	the same connection at once. Files MUST be locally queued instead.
628	
629	For example, if the Result Set from a QueryHit message contained
630	the entry
631	
632	    File Index: 2468
633	    File Size: 4356789
634	    File Name: Foobar.mp3
635	
636	then a download request for the file described by this entry would be
637	initiated as follows:
638	
639	    GET /2468/Foobar.mp3 HTTP/1.1<cr><lf>
640	    User-Agent: ROtella<cr><lf>
641	    Host: 123.123.123.123:6346<cr><lf>
642	    Connection: close<cr><lf>
643	    <cr><lf>
644	
645	Servents MUST encode the filename in GET requests according the 
646	standard URL/URI encoding rules. Servents MUST accept URL-encoded GET
647	requests.
648	
649	The Host header is required by HTTP 1.1 and specifies what address 
650	you have connected to. It is usually not used by the receiving 
651	servent, but its presence is required by the protocol.
652	
653	The allowable values of the User-Agent string are defined by the HTTP
654	standard. Servent developers cannot make any assumptions about the 
655	value here. The use of 'ROtella' is for illustration purposes only. 
656	
657	The server receiving this download request responds with HTTP 1.1 
658	compliant headers such as
659	
660	    HTTP/1.1 200 OK<cr><lf>
661	    Server: ROtella<cr><lf>
662	    Content-type: application/binary<cr><lf>
663	    Content-length: 4356789<cr><lf>
664	    <cr><lf>
665	
666	The file data then follows and should be read up to, and including, 
667	the number of bytes specified in the Content-length provided in the 
668	server's HTTP response.
669	
670	Headers unknown to the servent MUST be quietly ignored.
671	
672	
673	4.2 Sharing
674	
675	Servents that are able to download files MUST also be able to share 
676	files with others.
677	
678	Servents MUST NOT give precedence to other users using the same 
679	servent. They MUST answer Query messages and accept file download
680	requests using the same rules for all servents.
681	
682	Servents SHOULD avoid changing the index numbers of shared files.
683	
684	Servents MUST NOT share partially downloaded (incomplete) files as if
685	they were complete. This is often done by using a separate directory 
686	for incomplete downloads. When the download finishes, the file is 
687	move to the downloads directory (that should be shared).
688	
689	
690	5 Credits
691	
692	ROtella is a simplification of the Gnutella protocol, version 0.6.
693	It is used as the specification for the practical assigment in the
694	Redes de Ordenadores II (Computer Netwoks II) course in the Carlos III
695	University of Madrid. For more details see
696	http://www.it.uc3m.es/celeste/docencia/inf/rroo/
697	
698	This simplification has been made by Celeste Campo, with the help of
699	Iciar Martínez, Carlos García Rubio, Andrés Marín y Rafael Sánchez.
700	
701	For the original Gnutella protocol, copyright (C) 2002, Tor
702	Klingberg & Raphael Manfredi. All Rights Reserved. The Gnutella
703	specification can be downloaded fromr
704	http://rfc-gnutella.sourceforge.net/src/rfc-0_6-draft.html
