


#include "gtest/gtest.h"
#include <unordered_set>
#include <string>


class Message;
class Folder;


class Folder{
    friend class Message;
    public:

    void add_message(const Message& m){
        messages.insert(&m);
    }
    void remove_message(const Message& m){
        messages.erase(&m);
    }

    bool contains(const Message& m)const {
        return messages.count(&m)!=0;
    }
    int get_message_num(){
        return messages.size();
    }
    private:
    std::unordered_set<const Message*> messages;

}; 

class Message{

    public:
    Message(){
        
    }
    Message(const Message& other):content(other.content),folders_that_contains_me(other.folders_that_contains_me){
        //folders_that_contains_me=other.folders_that_contains_me;
        for(auto& f:folders_that_contains_me){
            const_cast<Folder*>(f)->add_message(*this);
        }

    }
    Message& operator=(const Message& other){
        content = other.content;
        folders_that_contains_me = other.folders_that_contains_me;
        for (auto &f : folders_that_contains_me)
        {
            const_cast<Folder *>(f)->add_message(*this);
        }
        return *this;
    }
//    ~Message(){
//        for (auto &f : folders_that_contains_me)
//        {
//            const_cast<Folder *>(f)->remove_message(*this);
//        }
//
//    }

    void save(Folder & folder){
        folders_that_contains_me.insert(&folder);
        folder .add_message(*this);
        //folder.messages.insert(this);
    }
    void rm(Folder& folder){
        folders_that_contains_me.erase(&folder);
        folder.remove_message(*this);
    }

    bool is_in(const Folder& f)const {
        return folders_that_contains_me.count(&f)!=0;
    }

    private:
    std::unordered_set<const Folder*> folders_that_contains_me;
    std::string content;


};


void expect_bind(const Message& message,const Folder& folder){

    EXPECT_TRUE(message.is_in(folder));
    EXPECT_TRUE(folder.contains(message));
}


void expect_not_bind(const Message& message,const Folder& folder){

    EXPECT_FALSE(message.is_in(folder));
    EXPECT_FALSE(folder.contains(message));
}


TEST(MessageAndFolderTest,test_save_message){
    Message m;
    Message m1;
    Folder f;
    m.save(f);

    expect_bind(m,f);
    expect_not_bind(m1,f);
}


TEST(MessageAndFolderTest,test_remove_message){
    std::vector<Message> messages(3);
    std::vector<Folder> folders(3);
    messages[0].save(folders[0]);
    messages[0].save(folders[1]);
    messages[0].rm(folders[1]);
    expect_bind(messages[0],folders[0]);
    expect_not_bind(messages[0],folders[1]);

}

TEST(MessageAndFolderTest,test_copy_message){
    Message m;
    std::vector<Folder> folders(2);
    m.save(folders[0]);
    Message m1(m);
    expect_bind(m1,folders[0]);
    expect_bind(m,folders[0]);
    m1.rm(folders[0]);
    expect_not_bind(m1,folders[0]);
    expect_bind(m,folders[0]);
}


TEST(MessageAndFolderTest,test_destructor){
    Message m;
    std::vector<Folder> folders(2);
    m.save(folders[0]);
//    {
//        Message m1(m);
//        expect_bind(m1, folders[0]);
//        expect_bind(m, folders[0]);
//    }
//        expect_bind(m, folders[0]);
        //EXPECT_EQ(folders[0].get_message_num(),1);
}
