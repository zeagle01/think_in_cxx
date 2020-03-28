


#include "gmock/gmock.h"
#include <memory>
#include <unordered_set>
#include <vector>
#include <string>

using namespace testing;

//class Message;
//class Folder;
//
//
//class Folder{
//    friend class Message;
//    public:
//
//    void add_message(const Message& m){
//        messages.insert(&m);
//    }
//    void remove_message(const Message& m){
//        messages.erase(&m);
//    }
//
//    bool contains(const Message& m)const {
//        return messages.count(&m)!=0;
//    }
//    int get_message_num(){
//        return messages.size();
//    }
//    private:
//    std::unordered_set<const Message*> messages;
//
//}; 
//
//class Message{
//
//    public:
//    Message(){
//        
//    }
//    Message(const Message& other):content(other.content),folders_that_contains_me(other.folders_that_contains_me){
//        //folders_that_contains_me=other.folders_that_contains_me;
//        for(auto& f:folders_that_contains_me){
//            const_cast<Folder*>(f)->add_message(*this);
//        }
//
//    }
//    Message& operator=(const Message& other){
//        content = other.content;
//        folders_that_contains_me = other.folders_that_contains_me;
//        for (auto &f : folders_that_contains_me)
//        {
//            const_cast<Folder *>(f)->add_message(*this);
//        }
//        return *this;
//    }
////    ~Message(){
////        for (auto &f : folders_that_contains_me)
////        {
////            const_cast<Folder *>(f)->remove_message(*this);
////        }
////
////    }
//
//    void save(Folder & folder){
//        folders_that_contains_me.insert(&folder);
//        folder .add_message(*this);
//        //folder.messages.insert(this);
//    }
//    void rm(Folder& folder){
//        folders_that_contains_me.erase(&folder);
//        folder.remove_message(*this);
//    }
//
//    bool is_in(const Folder& f)const {
//        return folders_that_contains_me.count(&f)!=0;
//    }
//
//    private:
//    std::unordered_set<const Folder*> folders_that_contains_me;
//    std::string content;
//
//
//};
//
//
//void expect_bind(const Message& message,const Folder& folder){
//
//    EXPECT_TRUE(message.is_in(folder));
//    EXPECT_TRUE(folder.contains(message));
//}
//
//
//void expect_not_bind(const Message& message,const Folder& folder){
//
//    EXPECT_FALSE(message.is_in(folder));
//    EXPECT_FALSE(folder.contains(message));
//}
//
//
//TEST(MessageAndFolderTest,test_save_message){
//    Message m;
//    Message m1;
//    Folder f;
//    m.save(f);
//
//    expect_bind(m,f);
//    expect_not_bind(m1,f);
//}
//
//
//TEST(MessageAndFolderTest,test_remove_message){
//    std::vector<Message> messages(3);
//    std::vector<Folder> folders(3);
//    messages[0].save(folders[0]);
//    messages[0].save(folders[1]);
//    messages[0].rm(folders[1]);
//    expect_bind(messages[0],folders[0]);
//    expect_not_bind(messages[0],folders[1]);
//
//}
//
//TEST(MessageAndFolderTest,test_copy_message){
//    Message m;
//    std::vector<Folder> folders(2);
//    m.save(folders[0]);
//    Message m1(m);
//    expect_bind(m1,folders[0]);
//    expect_bind(m,folders[0]);
//    m1.rm(folders[0]);
//    expect_not_bind(m1,folders[0]);
//    expect_bind(m,folders[0]);
//}
//
//
//TEST(MessageAndFolderTest,test_destructor){
//    Message m;
//    std::vector<Folder> folders(2);
//    m.save(folders[0]);
////    {
////        Message m1(m);
////        expect_bind(m1, folders[0]);
////        expect_bind(m, folders[0]);
////    }
////        expect_bind(m, folders[0]);
//        //EXPECT_EQ(folders[0].get_message_num(),1);
//}

/////////////////////////////new ///////////////////////////////////

//
namespace messge_and_folder
{
    class Folder;
    class Message: public std:: enable_shared_from_this<Message>
    {
    public:
        Message() = default;
        Message(const Message& other);

        void operator=(const Message &other)
        {
            //TODO
            //m_content = other.m_content;
            //m_folders = other.m_folders;
        }

        void save(std::shared_ptr<Folder> folder);
        void remove_from(std::shared_ptr<Folder> folder);
        std::string get_content() const;
        void set_content(const std::string &content);

        std::shared_ptr<Message> get_shared_deep_copy()
        {
            std::shared_ptr<Message> ret = std::make_shared<Message>(*this);
            return ret;
        }
    private:
        std::string m_content;
        std::vector<std::shared_ptr<Folder>> m_folders;
    };

    class Folder
    {
        public:
        std::vector<std::string> Folder::get_all_contents() const;
        void add_message(std::shared_ptr<Message> message);
        void remove_message(std::shared_ptr<Message> message);

    private:
        std::vector<std::shared_ptr<Message>> m_messages;
    };

    void Folder::add_message(std::shared_ptr<Message> message)
    {
        m_messages.push_back(message);
    }

    void Folder::remove_message(std::shared_ptr<Message> message)
    {
        for(size_t i=0;i<m_messages.size();i++)
        {
            if (m_messages[i] == message)
            {
                m_messages.erase(m_messages.begin() + i);
                break;
            }
        }
    }

    std::vector<std::string> Folder::get_all_contents()const 
    {
        std::vector<std::string> ret;
        for(auto it:m_messages)
        {
            ret.push_back(it->get_content());
        }
        return ret;
    }

//////////////////////Message///////////////////////////

    Message::Message(const Message &other)
    {
        m_content = other.m_content;
        m_folders = other.m_folders;
        for (auto it : m_folders)
        {
            it->add_message(shared_from_this());
        }
    }
    void Message::save(std::shared_ptr<Folder> folder)
    {
        folder->add_message(shared_from_this());
        m_folders.push_back(folder);
    }

    void Message::remove_from(std::shared_ptr<Folder> folder)
    {
        folder->remove_message(shared_from_this());
    }

    std::string Message::get_content() const
    {
        return m_content;
    }

    void Message::set_content(const std::string& content)
    {
        m_content = content;
    }
}

class A_Message_Saved_By_Two_Folders_Test : public testing::Test
{

    protected:
    virtual void SetUp() override
    {
		message = std::make_shared<messge_and_folder::Message>();
        message->set_content("hello world!");

        folder1 = std::make_shared<messge_and_folder::Folder>();
        folder2 = std::make_shared<messge_and_folder::Folder>();
        folder3 = std::make_shared<messge_and_folder::Folder>();

        message->save(folder1);
        message->save(folder2);
        copy_message = message->get_shared_deep_copy();
    }

    virtual void TearDown() override
    {
        //message.
    }

    std::shared_ptr<messge_and_folder::Folder> folder1;
    std::shared_ptr<messge_and_folder::Folder> folder2;
    std::shared_ptr<messge_and_folder::Folder> folder3;

    std::shared_ptr<messge_and_folder::Message> message;
    std::shared_ptr<messge_and_folder::Message> copy_message;
};


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_content_is_not_equal_to_new_content)
{
    std::string exp("new content!");
    EXPECT_THAT(message->get_content(),Ne(exp));
}


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_set_content_as_expected)
{
    std::string exp("new content!");
    message->set_content(exp);
    EXPECT_THAT(message->get_content(),Eq(exp));
}


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_content_is_the_same)
{
    EXPECT_THAT(folder1->get_all_contents(),Eq(folder2->get_all_contents()));
}

TEST_F(A_Message_Saved_By_Two_Folders_Test,test_save_into_a_new_folder)
{
    message->save(folder3);
    EXPECT_THAT(folder3->get_all_contents(),Contains(message->get_content()));
}


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_delete_from_folder)
{
    message->remove_from(folder1);
    EXPECT_THAT(folder1->get_all_contents(),Not(Contains(message->get_content())));
}

TEST_F(A_Message_Saved_By_Two_Folders_Test,test_set_new_content_will_sync_in_folders)
{
    message->set_content("new content");
    EXPECT_THAT(folder1->get_all_contents(),Contains(message->get_content()));
    EXPECT_THAT(folder2->get_all_contents(),Contains(message->get_content()));
}


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_copied_message_has_same_content)
{
    EXPECT_THAT(copy_message->get_content(),Eq(message->get_content()));
}

TEST_F(A_Message_Saved_By_Two_Folders_Test,test_copied_message_contained_in_same_folders)
{
    EXPECT_THAT(folder1->get_all_contents(),Contains(copy_message->get_content()));
    EXPECT_THAT(folder2->get_all_contents(),Contains(copy_message->get_content()));
}


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_copied_message_set_different_content_wont_affect_original)
{
    std::string act("copy content");
    copy_message->set_content(act);
    EXPECT_THAT(message->get_content(),Ne(act));
}

TEST_F(A_Message_Saved_By_Two_Folders_Test,test_copied_message_set_different_content_will_sync_in_folders)
{
    std::string act("copy content");
    copy_message->set_content("copy content");
    EXPECT_THAT(folder1->get_all_contents(),Contains(act));
    EXPECT_THAT(folder2->get_all_contents(),Contains(act));
}
