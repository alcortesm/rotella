  1	Rotella Web Caching System
  2	
  3	Copyright Notice
  4	
  5	  ROtella Web Caching System is a simplification of the Gnutella Web
  6	  Caching System, version 1.3.1/6.7.2002. It is used as the
  7	  specification for the practical assigment in the Redes de
  8	  Ordenadores II (Computer Netwoks II) course in the Carlos III
  9	  University of Madrid. For more details see
 10	  http://www.it.uc3m.es/celeste/docencia/inf/rroo/
 11	
 12	  This simplification has been made by Celeste Campo, with the help of
 13	  Iciar Martínez, Carlos García Rubio, Andrés Marín y Rafael Sánchez.
 14	
 15	  For the original Gnutella Web Caching System, Copyright (c) 2003 Hauke
 16	  Dämpfling, version 1.3.1 / 6.7.2002,
 17	  http://www.gnucleus.com/gwebcache/
 18	
 19	Table of Contents
 20	
 21	  1 Introduction 
 22	  2 Client-Script Interface 
 23	     2.1 "Hostfile" Request 
 24	     2.2 Update Request  
 25	  3 Client-Side Behavior 
 26	     3.1 Requests 
 27	  4 Server-Side Behavior 
 28	
 29	
 30	1 Introduction
 31	
 32	The goal of the "Rotella Web Caching System" (the "cache") is to
 33	eliminate the "Initial Connection Point Problem" of a fully
 34	decentralized network: Where do I find a first host to connect to? The
 35	cache is a program (script) placed on any web server that stores IP
 36	addresses of hosts in the Rotella network and URLs of other
 37	caches. Rotella clients connect to a cache in their list
 38	randomly. They send and receive IP addresses and URLs from the
 39	cache. With the randomized connection it is to be assured that all
 40	caches eventually learn about each other, and that all caches have
 41	relatively fresh hosts and URLs. The concept is independent from
 42	Rotella clients.
 43	
 44	Caches should only return a few (around 10-20) and only the newest
 45	Hosts. Therefore they only need to keep that many entries in their
 46	lists and can flush older entries as newer ones arrive. Keeping such a
 47	relatively small number of entries is essential to keeping all the
 48	information in all caches fresh.
 49	
 50	2 Client-Script Interface
 51	
 52	2.1 "Hostfile" Request
 53	
 54	The client wishes to receive a list of Rotella nodes.
 55	
 56	Request:   URL?hostfile=1
 57	
 58	Response:  A HTTP response code 2xx with a plain text, return-separated 
 59	           list of Rotella nodes in the format "ip:port" (numerical IPs only)
 60	           in the data section of the HTTP response. The list should not be very
 61		   long (around 20 nodes) and should contain only the newest
 62		   entries. (Returning an empty list is also possible in case
 63		   the cache does not have any hosts stored yet).
 64	
 65		   OR
 66	
 67		   A redirect (HTTP code 3xx) response, indicating that the
 68		   client needs to send another HTTP GET request for the
 69		   file. Clients must support this method. Luckily, many
 70		   standard HTTP libraries automatically follow
 71		   redirects. When a client follows the redirect, it should
 72		   receive a list as described above.
 73	
 74		   OR
 75	
 76		   The string "ERROR", possibly followed by more specific
 77		   error information.
 78	
 79	2.2 Update Request
 80	
 81	The client wishes to update IP addresses and/or alternate web cache
 82	URLs to a cache.
 83	
 84	Request:  URL?ip=XXX.XXX.XXX.XXX:PORT 
 85		  Reminder: Requests need to be URL-Encoded - see "Requests" in 
 86		  "Client-Side Behavior"
 87	
 88	Response: A HTTP response code 2xx with a plain text, which first line must 
 89	          be: either "OK" or "ERROR: Message". The following lines can be 
 90	          used by the script for further messages, such as information or 
 91	          warnings (can be ignored by the client).
 92	
 93	3 Client-Side Behavior
 94	
 95	Clients generally keep an internal cache of the IP addresses of known
 96	Rotella nodes.
 97	
 98	In addition to this list, they should also keep an internal list of
 99	web caches. 
100	
101	When making requests, a client should pick a cache from its internal
102	list - a different one every time.
103	
104	A client should send a Hostfile request whenever it needs hosts to
105	connect to.
106	
107	After a client has been up for an hour, it should begin sending an
108	Update request periodically - every 60 minutes. It sends its own IP
109	address and port in the "ip" parameter and a the URL of a random cache
110	in the "url" parameter. 
111	
112	3.1 Requests
113	
114	Interaction with the web server and cache is a series of HTTP GET
115	requests and responses. In these specifications, notation URL?query
116	indicates the URLs of a script with the attached query string, where
117	"query" is a series of name=value pairs joined by "&"s. These
118	name/value pairs must be "URL-Encoded".
119	
120	Due to the differences between operating systems, responses can be LF,
121	CRLF, or CR-terminated, but should be of Content-Type
122	"text/*". Responses are interpreted line-by-line.
123	
124	Tip: GET requests: the query (the information/request you are sending
125	the script) is simply part of the URL. For example, let's say the
126	request is: URL?ip=192.168.0.1:123, you will simply have to open the
127	following URL using whatever web functions your programming language
128	provides:
129	
130	http://www.somehost.com/path/to/script.php?ip=192.168.0.1:123
131	
132	4 Server-Side Behavior
133	
134	Scripts accept requests by clients through the standard HTTP GET
135	mechanism and should respond according to these specifications;
136	otherwise they risk being marked as invalid by a client. Responses
137	should be of Content-Type "text/*" (i.e. any content-type that is text
138	- such as text/plain or text/html).
139	
140	An OK message usually means that everything went well and the script
141	executed normally.
142	
143	An ERROR message usually indicates some form of fatal error because of
144	which the script could not do what is supposed to. Since clients will
145	(should) remove scripts that return error messages often, it is
146	advised to return ERRORs only when the script is expected to be down
147	for a while (such as, the script will be or has been removed from
148	server, server overload, file errors, etc.).
149	
150	In other words, things such as the submission of an invalid IP and/or
151	URL, should more likely be answered by an Error followed by a warning
152	message on the next line(s).
153	
154	Scripts should only return a few (around 10-20) and only the newest
155	Hosts. Therefore they only need to keep that many entries in their
156	lists and can flush older entries as newer ones arrive. Keeping such a
157	relatively small number of entries is essential to keeping all the
158	information in all caches fresh.
159	
160	
161	
162	
163	
164	
165	
166	
167	
168	
169	
170	
171	
172	
173	
174	
