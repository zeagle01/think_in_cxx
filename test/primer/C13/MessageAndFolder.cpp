


#include "gmock/gmock.h"
#include <memory>
#include <unordered_set>
#include <vector>
#include <string>

using namespace testing;
namespace messge_and_folder
{


//smart pointer is complicated in dealing with mutual reference
//1. enable_from_this
//2. can't use shared_from_this in copy_constructor 

    class Folder;
    class Message: public std:: enable_shared_from_this<Message>
    {
    public:
        Message() 
        {
        };
        Message(const Message& other);

        Message &operator=(const Message &other) = default;
        //        {
        //            //TODO
        //            m_content = other.m_content;
        //            m_folders = other.m_folders;
        //        }

        void save(Folder& folder);
        void remove_from(Folder& folder);
        std::string get_content() const;
        void set_content(const std::string &content);
    private:
        std::string m_content;
        std::vector<Folder*> m_folders;
    };

    class Folder
    {
        public:
        std::vector<std::string> Folder::get_all_contents() const;
        void add_message(Message& message);
        void remove_message(Message& message);

    private:
        std::vector<Message*> m_messages;
    };

    void Folder::add_message(Message& message)
    {
        m_messages.push_back(&message);
    }

    void Folder::remove_message(Message& message)
    {
        for(size_t i=0;i<m_messages.size();i++)
        {
            if (m_messages[i] == &message)
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
            it->add_message(*this);
        }
    }
    void Message::save(Folder& folder)
    {
        folder.add_message(*this);
        m_folders.push_back(&folder);
    }

    void Message::remove_from(Folder& folder)
    {
        folder.remove_message(*this);
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
        message.set_content("hello world!");
        message.save(folder1);
        message.save(folder2);

        copy_message = message;
    }

    virtual void TearDown() override
    {
        //message.
    }

    messge_and_folder::Folder folder1;
    messge_and_folder::Folder folder2;
    messge_and_folder::Folder folder3;

    messge_and_folder::Message message;
    messge_and_folder::Message copy_message;
};


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_content_is_not_equal_to_new_content)
{
    std::string exp("new content!");
    EXPECT_THAT(message.get_content(),Ne(exp));
}


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_set_content_as_expected)
{
    std::string exp("new content!");
    message.set_content(exp);
    EXPECT_THAT(message.get_content(),Eq(exp));
}


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_content_is_the_same)
{
    EXPECT_THAT(folder1.get_all_contents(),Eq(folder2.get_all_contents()));
}

TEST_F(A_Message_Saved_By_Two_Folders_Test,test_save_into_a_new_folder)
{
    message.save(folder3);
    EXPECT_THAT(folder3.get_all_contents(),Contains(message.get_content()));
}


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_delete_from_folder)
{
    message.remove_from(folder1);
    EXPECT_THAT(folder1.get_all_contents(),Not(Contains(message.get_content())));
}

TEST_F(A_Message_Saved_By_Two_Folders_Test,test_set_new_content_will_sync_in_folders)
{
    message.set_content("new content");
    EXPECT_THAT(folder1.get_all_contents(),Contains(message.get_content()));
    EXPECT_THAT(folder2.get_all_contents(),Contains(message.get_content()));
}


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_copied_message_has_same_content)
{
    EXPECT_THAT(copy_message.get_content(),Eq(message.get_content()));
}

TEST_F(A_Message_Saved_By_Two_Folders_Test,test_copied_message_contained_in_same_folders)
{
    EXPECT_THAT(folder1.get_all_contents(),Contains(copy_message.get_content()));
    EXPECT_THAT(folder2.get_all_contents(),Contains(copy_message.get_content()));
}


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_copied_message_set_different_content_wont_affect_original)
{
    std::string act("copy content");
    copy_message.set_content(act);
    EXPECT_THAT(message.get_content(),Ne(act));
}

TEST_F(A_Message_Saved_By_Two_Folders_Test,test_copied_message_set_different_content_will_sync_in_folders)
{
    std::string act("copy content");
    copy_message.set_content(act);
    EXPECT_THAT(folder1.get_all_contents(),Contains(act));
    EXPECT_THAT(folder2.get_all_contents(),Contains(act));
}
