#API

## POST

### request.json

```
{  
   "request":[  
      {  
         "name":"serverInfo"
      },
      {  
         "name":"players",
         "data":{  
            "sort":"DESC"
         }
      }
   ]
}
```
Response
```
{  
   "response":[  
      {  
         "name":"serverInfo",
         "data":{  
            "servername":"",
            "motd":""
         }
      },
      {  
         "name":"players",
         "data":[  
            {  
               "id":"",
               "name":"",
               "kills":"",
               "deaths":""
            }
         ]
      }
   ]
}
```

### action.json

```
{  
   "action":[  
      {  
         "name":"kickplayer",
         "data":{  
            "id":"",
            "instanceid":""
         }
      },
      {  
         "name":"banplayer",
         "data":{  
            "id":""
         }
      }
   ]
}
```
Response

-------
## GET

#### request.json
Response
```
{  
   "servertype":"Lobby/Dedi/Unknown"
}
```