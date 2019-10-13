#include <bits/stdc++.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include<string>
#include<arpa/inet.h>
#include<pthread.h>
using namespace std;
struct clientDetails {
  string user_id;
  string passwd;
  string ipaddr;
  int port;
  char* oldfile;
  char* newfile;
  int fd;
  int chunks;
  int online=0;
};


struct groupDetails{
  string group_id;
  vector<string>files;
  string hash;
  string  group_owner;
  vector<string>users;
  vector<string>pending_requests;
  int nochunks;
};

struct file{
  string sha;
  int file_size;
  vector<string>users;
};

//key = group_id
map<string,struct groupDetails *>groups;
//key = gid+filename
map<string,struct file *>gidfile;
//key = user_id
map<string,struct clientDetails *>details;

//
// struct groupDetails{
//   string group_id;
//   string  group_owner;
//   vector<string>users;
// };


void *print_message_fucntion(void *ptr){
  char *message;
  int c;
  message = (char *)ptr;
  printf("%s \n",message );
  //cin>>c;
}

void* clientThreadFunc(void* threadarg){
  //create a Socket
  int sockfd = socket(AF_INET,SOCK_STREAM,0);
  if(sockfd < 0){
    perror("error while creating socket");
    exit(1);
  }
  //create a hint structure for server we're connecting to
  int port;
  cin>>port;
  // string ipAddress = "127.0.0.1";
  struct sockaddr_in hint;
  hint.sin_family = AF_INET;
  hint.sin_port = htons(port);
  hint.sin_addr.s_addr = INADDR_ANY;


  //inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
  //connect to server on Socket
  int connRes = connect(sockfd,(struct sockaddr*)&hint,sizeof(hint));
  if(connRes<0){
    perror("couldnt connect with the server");
    exit(1);
  }
  //send data
  char buf[4096];
  memset(buf,0,4096);
  FILE *fp;
  fp = fopen("/home/ubuntu/Desktop/Desktop/testing.txt","wb");
  int file_size;
  int byteRec = recv(sockfd,&file_size,sizeof(file_size),0);
  int n;
  while((n = recv(sockfd,buf,1024,0))>0 && file_size>0){
    fwrite(buf,sizeof(char), n, fp);
    memset(buf,'\0',1024);
    file_size = file_size-n;
  }
   fclose(fp);
   // string userip;
   // do{
   //   cout<<">";
   //   getline(cin,userip);
   //   int serversend = send(sockfd,userip.c_str(),userip.size()+1,0);
   //   if(serversend<0){
   //     perror("couldnt send data to server");
   //     continue;
   //   }
   //   //waitfor server response
   //   memset(buf,0,4096);
   //   int byteRec = recv(sockfd,buf,4096,0);
   //   //int byteRec = recv(sockfd,buf,4096,0);
   //   if(byteRec < 0){
   //     perror("connection issue");
   //     break;
   //   }
   //   if(byteRec == 0){
   //     perror("client disconnected ");
   //     break;
   //   }
   //   cout<<"received : "<<string(buf,0,byteRec)<<endl;
   //
   // }while(true);
  close(sockfd);

}

void* FileRecFunc(void* threadarg){
  char host[NI_MAXHOST];
  struct clientDetails *th;
  int flag;
  char svc[NI_MAXSERV];
  string username;
  memset(host,0,NI_MAXHOST);
  memset(svc,0,NI_MAXSERV);
  cout<<"entered in FileRecFunc"<<endl;
  th = (struct clientDetails *)threadarg;
  int clientSocket = th->fd;
  char buf[4096];
  memset(buf,0,4096);
  // int byteRec = recv(clientSocket,buf,4096,0);
  // if(byteRec < 0){
  //   perror("connection issue");
  // //  exit(1);
  // }
  // if(byteRec == 0){
  //   perror("client disconnected ");
  // //  exit(1);
  // }
  // string receive = string(buf,0,byteRec);
  // cout<<"received- : "<<receive<<endl;
  // char input_char[1024];
  // strcpy(input_char,receive.c_str());
  // char *temp1[1024];
  // int k =0;
  // temp1[k++] = strtok(input_char,":");
  // int csend;
  //
  // //push the rest of the text into the array till EOF
  // while((temp1[k] = strtok(NULL,":")) != NULL)
  //   k++;
  //   temp1[k] = NULL;
  //   cout<<"temp1[0]  "<<temp1[0]<<endl;

string tempuser_id;

    while(1){

      struct clientDetails *temporary;
    //  memset(buf,0,4096);
      int byteRec = recv(clientSocket,buf,4096,0);
      if(byteRec < 0){
        perror("connection issue");
      //  exit(1);
      }
      if(byteRec == 0){
        perror("client disconnected ");
      //  exit(1);
      }
      string receive = string(buf,0,byteRec);
      cout<<"received- : "<<receive<<endl;
      char input_char[1024];
      strcpy(input_char,receive.c_str());
      char *temp1[1024];
      int k =0;
      temp1[k++] = strtok(input_char,":");
      int csend;

      //push the rest of the text into the array till EOF
      while((temp1[k] = strtok(NULL,":")) != NULL)
        k++;
        temp1[k] = NULL;
        cout<<"temp1[0]  "<<temp1[0]<<endl;

  if(strcmp(temp1[0],"create_user") == 0){
    cout<<"user_id"<<temp1[1]<<endl;
    cout<<"passwd"<<temp1[2]<<endl;



    struct clientDetails *user_entry;
    user_entry = (struct clientDetails*)malloc(sizeof(clientDetails));
    user_entry->user_id = temp1[1];
    username = temp1[1];
    user_entry->passwd = temp1[2];
    user_entry->fd = clientSocket;
    user_entry->ipaddr = th->ipaddr;
    user_entry->port = th->port;
    user_entry->online = 0;
    char *temp;
    temp = temp1[1];
    string tempuser_id_1;
    tempuser_id_1=temp;
    cout<<"string is "<<tempuser_id_1<<endl;

    //MAKING ENTRY IN MAP
    details[tempuser_id_1] = user_entry;
    string res = "Create user successful";//
    //cout<<details->first<<endl;
    for(auto itr = details.begin(); itr != details.end(); ++itr){
      cout<<itr->first<<endl;

    }
    csend = send(clientSocket,(char*)res.c_str(),1024,0);

  }
  else if(strcmp(temp1[0],"login") == 0){

    cout<<"received login"<<endl;
    char *temp;
    temp = temp1[1];
  //  string tempuser_id;
    tempuser_id=temp;
    username = tempuser_id;
    if(details.find(tempuser_id) == details.end()){
      string res = "doesnt exist";
      cout<<res<<endl;
    }
    else{
    //  flag = 1;

      cout<<"client exists "<<endl;
      temporary = details.at(tempuser_id);
      if(strcmp((temporary->passwd).c_str(),temp1[2]) == 0){
        string res = "Login successful";
        temporary->online = 1;
         flag = 1;
        cout<<"res = "<<res<<endl;
        csend = send(clientSocket,(char*)res.c_str(),1024,0);
      }
      else{
        string res = "Login unsuccessful";
        cout<<"res = "<<res<<endl;
         flag =0;
        csend = send(clientSocket,(char*)res.c_str(),1024,0);
      }
    }
  }
  else if(strcmp(temp1[0],"create_group") == 0){


          string tempgroup = temp1[1];
          struct groupDetails *group_entry;
          group_entry = (struct groupDetails*)malloc(sizeof(groupDetails));

          group_entry->group_id = tempgroup;
          group_entry->group_owner = username;
          group_entry->users.push_back(tempuser_id);
          // group_entry->users.push_back("hello");
          // group_entry->users.push_back("hi");

          groups[tempgroup]  = group_entry;
          // group.group_id = tempgroup;
          // group.group_owner = tempuser_id;
          // group.users.push_back(temporary);
          // groups[tempgroup] = group;
          string res = "Group Created successfully";
          cout<<"res = "<<res<<endl;

      //    int flag =0;
          csend = send(clientSocket,(char*)res.c_str(),1024,0);


      }
      else if(strcmp(temp1[0],"join_group")==0){

          string tempgroup = temp1[1];
          cout<<"group name=  "<<tempgroup<<endl;
          struct groupDetails *group_entry;
          group_entry = (struct groupDetails*)malloc(sizeof(groupDetails));
          struct groupDetails *iter=(struct groupDetails*)malloc(sizeof(groupDetails));;

          string res;
          int i;

          if(groups.find(tempgroup) == groups.end()){
            res = "doesnt exist";
            cout<<res<<endl;
          }
          else{
              iter = groups[tempgroup];
               int n = iter->pending_requests.size();
               cout<<" n = "<<n<<endl;
               for(i=0;i<n;i++){
                 if(strcmp(iter->pending_requests[i].c_str(),tempuser_id.c_str()) == 0){
                    res = "user is already waiting";
                   cout<<"res = "<<res<<endl;
               //    int flag =0;

                   break;
               }
             }
          if(i == n && temporary->online == 0 && i!=0){
            iter->pending_requests.push_back(tempuser_id);
             res = "user is added to pending_requests";
            cout<<"res = "<<res<<endl;

          }
          else if(i==0){
            iter->pending_requests.push_back(tempuser_id);
            cout<<"first user added to pending list"<<endl;
          }
        }
           csend = send(clientSocket,(char*)res.c_str(),1024,0);




      }
      else if(strcmp(temp1[0],"leave_group")==0){
        string tempgroup = temp1[1];
        struct groupDetails *group_entry;
        group_entry = (struct groupDetails*)malloc(sizeof(groupDetails));
        struct groupDetails *iter=(struct groupDetails*)malloc(sizeof(groupDetails));;
        iter = groups[tempgroup];
        string res;
        int i;

        int n = iter->users.size();

      //searching for existence of user in the group
        for( i=0;i<n;i++){
          if(strcmp(iter->users[i].c_str(),username.c_str()) == 0){

            break;
          }
        }
        if(i==n){
          res = "User not found";
          cout<<"res = "<<res<<endl;
      //    int flag =0;
          csend = send(clientSocket,(char*)res.c_str(),1024,0);
        }else{
          //user found here
          vector<string>::iterator it;
          it = iter->users.begin()+i;
          iter->users.erase(it);
          res = "User removed successfully";
              csend = send(clientSocket,(char*)res.c_str(),1024,0);
        }

      }
      else if(strcmp(temp1[0],"list_requests")==0){
          string tempgroup = temp1[1];
        struct groupDetails *group_entry;
        cout<<"list groups"<<endl;
        group_entry = (struct groupDetails*)malloc(sizeof(groupDetails));
        struct groupDetails *iter=(struct groupDetails*)malloc(sizeof(groupDetails));
        //map<string,struct groupDetails*>:: iterator itrr;
        iter = groups[tempgroup];
        string res;
        int i;

        if(groups[tempgroup]->group_owner == temporary->user_id){

          int n = iter->pending_requests.size();
          cout<<" n =  "<<n<<endl;
          vector<string>::iterator it;

        //searching for existence of user in the group
          for( i=0;i<n;i++){

            cout<<"user_id =  "<<iter->pending_requests[i]<<endl;

            }
             res = "list printed successfully";


        }  else{
            //  the user logged in is not the owner of the group
            res = "Only the owner of the group can perform this";
            cout<<res<<endl;
                csend = send(clientSocket,(char*)res.c_str(),1024,0);
          }



      }
      else if(strcmp(temp1[0],"accept_requests")==0){
        string tempgroup = temp1[1];
        string tempuser = temp1[2];
      struct groupDetails *group_entry;
      cout<<"list groups"<<endl;
      group_entry = (struct groupDetails*)malloc(sizeof(groupDetails));
      struct groupDetails *iter=(struct groupDetails*)malloc(sizeof(groupDetails));
      //map<string,struct groupDetails*>:: iterator itrr;
      iter = groups[tempgroup];
      string res;
      int i;

      int n = iter->pending_requests.size();
      cout<<" n =  "<<n<<endl;
      vector<string>::iterator it;

      // -- first check if the user is owner or not ----

      if(groups[tempgroup]->group_owner == temporary->user_id){

        //  int n = iter->users.size();

        //searching for existence of user in the group
          for( i=0;i<n;i++){
            if(strcmp(iter->pending_requests[i].c_str(),tempuser.c_str()) == 0){

              break;
            }
          }
          if(i==n){
            res = "User not found in pending request queue";
            cout<<"res = "<<res<<endl;
        //    int flag =0;
            csend = send(clientSocket,(char*)res.c_str(),1024,0);
          }else{
            //user found here
            vector<string>::iterator it;
            it = iter->pending_requests.begin()+i;
            iter->pending_requests.erase(it);
            res = "User removed successfully from pending requests queue";
                csend = send(clientSocket,(char*)res.c_str(),1024,0);

                // --------user removed from pending_requests queue

                //----add user to the users vector in the gid structure

                  iter->users.push_back(tempuser);

                  int usersize = iter->users.size();

                  for(int i=0;i<usersize;i++){
                    cout<<"users in group = "<<iter->users[i]<<endl;
                  }

          }





      }
      else{
        //  the user logged in is not the owner of the group
        res = "Only the owner of the group can perform this";
        cout<<res<<endl;
            csend = send(clientSocket,(char*)res.c_str(),1024,0);
      }









      }
      else if(strcmp(temp1[0],"list_groups")==0){
      //  string tempgroup = temp1[1];
        struct groupDetails *group_entry;
        cout<<"list groups"<<endl;
        group_entry = (struct groupDetails*)malloc(sizeof(groupDetails));
        map<string,struct groupDetails*>:: iterator itrr;

        for(itrr=groups.begin();itrr!=groups.end();itrr++)
        {
          cout<<itrr->first<<endl;
        }



      }
      else if(strcmp(temp1[0],"list_files")==0){


      }
      else if(strcmp(temp1[0],"upload_file")==0){
        string filepath = temp1[1];
        string group_id = temp1[2];

        int byteRec = recv(clientSocket,buf,4096,0);
        if(byteRec < 0){
          perror("connection issue");
          exit(1);
        }
        if(byteRec == 0){
          perror("client disconnected ");
          exit(1);
        }
        string receive = string(buf,0,byteRec);
        cout<<"received- : "<<receive<<endl;


        cout<<filepath<<endl;







      }
      else if(strcmp(temp1[0],"download_file")==0){


      }
      else if(strcmp(temp1[0],"logout")==0){
        flag= 0;
        temporary->online = 0;
        string res;
        res = "User logout successfully";
        cout<<"res = "<<res<<endl;
    //    int flag =0;
        csend = send(clientSocket,(char*)res.c_str(),1024,0);


      }
      else if(strcmp(temp1[0],"show_downloads")==0){


      }
      else if(strcmp(temp1[0],"stop_share")==0){


      }
       memset(buf,0,4096);
    }

//     }
// }
//
//   }

  // if(strcmp(buf,"checkexistence") == 0){
  //   //call the checkexistencefucntion
  //    memset(buf,'\0',4096);
  //   cout<<"checking for existence";
  //   string check = checkexistence(data,clientip);
  //   char *arr;
  //   arr = (char* )check.c_str();
  //   cout<<"check = "<<check<<endl;
  //     send(clientSocket, arr, strlen(arr),0);
  //     if(check == "0"){
  //        memset(buf,'\0',4096);
  //        recv(clientSocket,buf,4096,0);
  //       //  cout<<"received- : "<<string(buf,0,byteRec)<<endl;
  //         string username = (string)buf;
  //         //  memset(buf,0,4096);
  //          memset(buf,'\0',4096);
  //         recv(clientSocket,buf,4096,0);
  //         string password = (string)buf;
  //
  //         cout<<username<<" "<<password<<endl;
  //       //  addtouserstruct(username,password);
  //         addtouserstruct(data,username,password,clientip,clientport);
  //          memset(buf,'\0',4096);
  //          char signedup[]="1";
  //          send(clientSocket, signedup, strlen(signedup),0);
  //     }
  //     //checkforlogin
  //     memset(buf,'\0',4096);
  //     recv(clientSocket,buf,4096,0);
  //     string username = (string)buf;
  //     //  memset(buf,0,4096);
  //      memset(buf,'\0',4096);
  //     recv(clientSocket,buf,4096,0);
  //     string password = (string)buf;
  //
  //     cout<<username<<" "<<password<<endl;
  //   //  addtouserstruct(username,password);
  //     // addtouserstruct(data,username,password,clientip,clientport);
  //     //  memset(buf,'\0',4096);
  //     //  char signedup[]="1";
  //     //  send(clientSocket, signedup, strlen(signedup),0);
  //     string res = checkcredentials(data,username, password);
  //     arr = (char* )res.c_str();
  //      send(clientSocket, arr, strlen(arr),0);
// }

/*    //while receiving,display message
   char buf[4096];
    FILE *fp;
    fp = fopen("/home/ubuntu/Desktop/Desktop/ss.txt","rb");
    fseek(fp,0,SEEK_END);
    int size = ftell(fp);
    rewind(fp);
    int serversend = send(clientSocket,&size,sizeof(60),0);
    // int n;
    // while((n=fread(buf,sizeof(char),1024,fp))>0 && size>0){
    //   serversend = send(clientSocket,buf,n,0);
    //     if(serversend<0){
    //       perror("couldnt send data to server");
    //       continue;
    //     }
    //     memset(buf,'\0',1024);
    //     size = size - n;
    // }
    fclose(fp);*/
close(clientSocket);
}

void* serverThreadFunc(void* threadarg){
  int opt = 1;

  int PORT;
  // cout<<"enter port";
  // cin>>PORT;
  PORT = 7385;
  //create a socket
  int server_sock;
  server_sock = socket(AF_INET,SOCK_STREAM,0);
  if(server_sock<0){
    perror("Socket creation failed");
    exit(1);
  }
  struct sockaddr_in hint;
  hint.sin_family = AF_INET;
  hint.sin_port = htons(PORT);
  //hint.sin_addr.s_addr = INADDR_ANY;
  int addrlen = sizeof(hint);
  inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);
//  hint.sin_addr.s_addr = INADDR_ANY;

  //bind the socket to an ip/PORT

  if(bind(server_sock,(sockaddr*)&hint,sizeof(hint)) == -1){
    perror("error while binding");
    exit(1);
  }
  if(listen(server_sock,4 ) == -1){
    perror("error occured while listening");
    exit(1);
  }

  //mark the socket for listening
  // struct sockaddr_in client;
  // socklen_t clientSize = sizeof(client);
  char host[NI_MAXHOST];
  char svc[NI_MAXSERV];

  int clientSocket;
  cout<<"ready to accept"<<endl;
  while((clientSocket = accept(server_sock,(struct sockaddr*)&hint,(socklen_t*)&addrlen))){
    cout<<"in accept"<<endl;
    pthread_t newThread;
    struct clientDetails th;

    char *ipinput = new char[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(hint.sin_addr),ipinput,INET_ADDRSTRLEN);
    string ipip = ipinput;
    int ipport = ntohs(hint.sin_port);
    th.ipaddr = ipip;
    th.port = ipport;
    // string socketRec = ipip+":"+ipport;
    //
    // cout<<"Socket "

    th.fd = clientSocket;
    cout<<"out from here"<<endl;
    int* ptrTopass = new int(clientSocket);
    pthread_create(&newThread, NULL, FileRecFunc, (void*)&th);
    cout<<"thread created"<<endl;
    pthread_detach(newThread);
    // cout<<"out from here"<<endl;
  }
  // if(clientSocket <0){
  //   perror("couldn't establish connection");
  //   exit(1);
  // }

  //close the listening socket

  // while(true){
  //   memset(buf,0,4096);
  //   int byteRec = recv(clientSocket,buf,4096,0);
  //   if(byteRec < 0){
  //     perror("connection issue");
  //     break;
  //   }
  //   if(byteRec == 0){
  //     perror("client disconnected ");
  //     break;
  //   }
  //   cout<<"received : "<<string(buf,0,byteRec)<<endl;
  //   send(clientSocket,buf,byteRec+1,0);
  // }
  //close socket
    close(clientSocket);
close(server_sock);

}


int main(){
  pthread_t clientThread,serverThread;
  string msg1 = "Thread 1";
  char *message1 = const_cast<char*>(msg1.c_str());
  string msg2 = "Thread 2";
  char *message2 = const_cast<char*>(msg2.c_str());
  int iret1,iret2;


  /*create independent threads each of which will execute fucntion*/
  iret2 = pthread_create(&serverThread, NULL, serverThreadFunc, NULL);
//  iret1 = pthread_create(&clientThread, NULL, clientThreadFunc, NULL);


  /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */

    // pthread_join (clientThread, NULL);
    pthread_join (serverThread , NULL);
    // printf("thread1 %d\n", iret1);
       printf("thread2 %d\n", iret2);
        exit(0);

}