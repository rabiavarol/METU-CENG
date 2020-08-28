

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "VideoShare.hpp"
#include "User.hpp"

using namespace std;

void VideoShare::printAllVideos() {
    videos.print();
}

void VideoShare::printAllUsers() {
    users.print();
}

/* TO-DO: method implementations below */



VideoShare::VideoShare() {

}

VideoShare::~VideoShare() {

}

void VideoShare::createUser(const string &userName, const string &name, const string &surname) {

    users.insertNode(users.getHead(),User(userName,name,surname));

}

void VideoShare::loadUsers(const string &fileName) {

    ifstream usersFile;
    usersFile.open(fileName.data());

    if (usersFile.is_open()) {
        int current = 0;
        string line, username, name, surname;
        string delimeter=";";
        string token;
        size_t pos=0;

        while (getline(usersFile, line)) {

            while((pos = line.find(delimeter)) != string::npos ) {

                token = line.substr(0, pos);
                if (current == 0) {
                    username = token;
                    current++;
                }

                else if (current == 1) {
                    name = token;
                }
                line.erase(0, pos + delimeter.length());
            }
            surname=line;
            current = 0;
            createUser(username, name, surname);
        }
        usersFile.close();

    }

}

void VideoShare::createVideo(const string &title, const string &genre) {

    videos.insertNode(videos.getHead(),Video(title,genre));
}

void VideoShare::loadVideos(const string &fileName) {

    ifstream usersFile;
    usersFile.open(fileName.data());

    if (usersFile.is_open()) {

        string line, title, genre;
        string delimeter=";";
        string token;
        size_t pos=0;

        while (getline(usersFile, line)) {

            while((pos = line.find(delimeter)) != string::npos ) {

                token = line.substr(0, pos);

                title = token;

                line.erase(0, pos + delimeter.length());
            }

            genre=line;

            createVideo(title, genre);
        }
        usersFile.close();

    }

}

void VideoShare::addFriendship(const string &userName1, const string &userName2) {

    Node<User>* User1 = users.findNode(userName1);
    Node<User>* User2 = users.findNode(userName2);

    if(!User1){

        return;
    }

    if(!User2){

        return;
    }

    User1->getDataPtr()->addFriend(User2->getDataPtr());

    User2->getDataPtr()->addFriend(User1->getDataPtr());


}

void VideoShare::removeFriendship(const string &userName1, const string &userName2) {

    Node<User>* User1 = users.findNode(userName1);
    Node<User>* User2 = users.findNode(userName2);

    if(!User1){

        return;
    }

    if(!User2){

        return;
    }

    User1->getDataPtr()->removeFriend(User2->getDataPtr());

    User2->getDataPtr()->removeFriend(User1->getDataPtr());
}

void VideoShare::updateUserStatus(const string &userName, Status newStatus) {

    Node<User>* User1 = users.findNode(userName);
    User1->getDataPtr()->updateStatus(newStatus);

}

void VideoShare::subscribe(const string &userName, const string &videoTitle) {

    Node<User>* User = users.findNode(userName);
    Node<Video>* Video = videos.findNode(videoTitle);

    if(!User){

        return;
    }

    if(User->getData().getStatus()==SUSPENDED){

        return;

    }

    User->getDataPtr()->subscribe(Video->getDataPtr());
}

void VideoShare::unSubscribe(const string &userName, const string &videoTitle) {

    Node<User>* User = users.findNode(userName);
    Node<Video>* Video = videos.findNode(videoTitle);

    if(User->getData().getStatus()==SUSPENDED){

        return;

    }

    User->getDataPtr()->unSubscribe(Video->getDataPtr());

}

void VideoShare::deleteUser(const string &userName) {

    Node<User>* User1 = users.findNode(userName);
    if(!User1){
        return;
    }
    Node<User*>* FriendOfUser= User1->getDataPtr()->getFriends()->first();

    while(FriendOfUser) {

        string NameOfFriend = FriendOfUser->getData()->getUsername();
        removeFriendship(userName, NameOfFriend);
        FriendOfUser=User1->getDataPtr()->getFriends()->first();
    }

    Node<User>* UserBack = users.findPrev(userName);

    users.deleteNode(UserBack);

}

void VideoShare::sortUserSubscriptions(const string &userName) {


    bool sorted = false;


    Node<Video*>* Video1 = users.findNode(userName)->getDataPtr()->getSubscriptions()->first();
    string titleOfVideo1 = users.findNode(userName)->getDataPtr()->getSubscriptions()->first()->getData()->getTitle();

    Node<Video*>* Video2 = users.findNode(userName)->getDataPtr()->getSubscriptions()->first()->getNext();
    string titleOfVideo2 = users.findNode(userName)->getDataPtr()->getSubscriptions()->first()->getNext()->getData()->getTitle();

    size_t lengthOfSubs = users.findNode(userName)->getDataPtr()->getSubscriptions()->getLength();

    for(int i=0 ; i < lengthOfSubs-1 && !sorted ; i++ ) {
        sorted = true;
        Video1 = users.findNode(userName)->getDataPtr()->getSubscriptions()->first();
        Video2 = users.findNode(userName)->getDataPtr()->getSubscriptions()->first()->getNext();

        for(int j=0;j < lengthOfSubs-1   ; j++) {

            titleOfVideo1=Video1->getData()->getTitle();
            titleOfVideo2=Video2->getData()->getTitle();

            if( titleOfVideo2.compare(titleOfVideo1) < 0 ) {
                users.findNode(userName)->getDataPtr()->getSubscriptions()->swap(j,j+1);
                sorted = false;
            }
            Video1=Video1->getNext();
            Video2=Video2->getNext();

        }
    }


}

void VideoShare::printUserSubscriptions(const string &userName) {
    Node<User>* User = users.findNode(userName);
    if(!User) {

        return;
    }

    User->getDataPtr()->printSubscriptions();

}

void VideoShare::printFriendsOfUser(const string &userName) {
    Node<User>* User = users.findNode(userName);
    if(!User) {

        return;
    }

    User->getDataPtr()->printFriends();
}

void VideoShare::printCommonSubscriptions(const string &userName1, const string &userName2) {

    LinkedList<Video* >* SubsOfUser1=users.findNode(userName1)->getDataPtr()->getSubscriptions();
    LinkedList<Video* >* SubsOfUser2=users.findNode(userName2)->getDataPtr()->getSubscriptions();


    Node<Video*>* Video1 = users.findNode(userName1)->getDataPtr()->getSubscriptions()->first();
    Node<Video*>* Video2 = users.findNode(userName2)->getDataPtr()->getSubscriptions()->first();




    while(Video1)
    {
        if(SubsOfUser2->findNode(Video1->getData()))
        {
            cout << *(Video1->getData()) ;
        }
        Video1=Video1->getNext();

    }


}

void VideoShare::printFriendSubscriptions(const string &userName) {

    LinkedList<Video> new_list = LinkedList<Video> ;
    LinkedList<Video* >* SubsOfUser=users.findNode(userName)->getDataPtr()->getSubscriptions();
    LinkedList<User* >* FriendsOfUser=users.findNode(userName)->getDataPtr()->getFriends()->first();

    Node<Video*>* videoOfUser = users.findNode(userName)->getDataPtr()->getSubscriptions()->first();

    while(FriendsOfUser) {
        while(SubsOfUser) {
            SubsOfUser->findNode(videoOfUser->getData())

        }


    }












}

bool VideoShare::isConnected(const string &userName1, const string &userName2) {


}

