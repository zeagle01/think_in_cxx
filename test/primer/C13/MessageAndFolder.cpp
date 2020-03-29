


#include "gmock/gmock.h"
#include <memory>
#include <unordered_set>
#include <string>
#include <optional>

using namespace testing;
namespace messge_and_folder
{


//smart pointer is complicated in dealing with mutual reference
//1. enable_from_this
//2. can't use shared_from_this in copy_constructor 

    class Folder;
    class Message 
    {
    public:
        Message() = default;
        virtual ~Message() ;
        Message(const Message& other);


        //Message &operator=(const Message &other) = default; //this is the default copy assigment
        Message &operator=(const Message &other) ;

        void save(Folder& folder);
        void remove_from(Folder& folder);
        std::string get_content() const;
        void set_content(const std::string &content);
    private:
        std::string m_content;
        std::unordered_set<Folder*> m_folders;

        void remove_from_folders();
        void add_to_folders();
    };

    class Folder
    {
        public:
        std::vector<std::string> Folder::get_all_contents() const;
        void add_message(Message& message);
        void remove_message(Message& message);

    private:
        std::unordered_set<Message*> m_messages;
    };

    void Folder::add_message(Message& message)
    {
        m_messages.insert(&message);
    }

    void Folder::remove_message(Message& message)
    {
        if(m_messages.count(&message))
        {
            m_messages.erase(&message);
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
        add_to_folders();
    }

    Message::~Message()
    {
        remove_from_folders();
    }

    Message& Message::operator=(const Message &other)
    {
        remove_from_folders();
        m_content = other.m_content;
        m_folders = other.m_folders;
        add_to_folders();
        return *this;
    }

    void Message::remove_from_folders()
    {
		if (!m_folders.empty())
		{
			for (auto it : m_folders)
			{
				it->remove_message(*this);
			}
		}
    }
    void Message::add_to_folders()
    {
        for (auto it : m_folders)
        {
            it->add_message(*this);
        }
    }
    void Message::save(Folder& folder)
    {
        folder.add_message(*this);
        m_folders.insert(&folder);
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

///////////////////////////////basic test/////////////////////////////////////////

class A_Message_Saved_By_Two_Folders_Test : public testing::Test
{

    protected:
    virtual void SetUp() override
    {
        message.set_content(init_content);
        message.save(folder1);
        message.save(folder2);

    }


    messge_and_folder::Folder folder1;
    messge_and_folder::Folder folder2;
    messge_and_folder::Folder folder3;

    messge_and_folder::Message message;
    std::string init_content = "init content!";
    std::string new_content = "new content!";
};



TEST_F(A_Message_Saved_By_Two_Folders_Test,test_new_content_is_not_equal_to_initial_content)
{
    EXPECT_THAT(message.get_content(),Ne(new_content));
}


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_set_content_as_expected)
{
    message.set_content(new_content);
    EXPECT_THAT(message.get_content(),Eq(new_content));
}


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_content_is_the_same)
{
    EXPECT_THAT(folder1.get_all_contents(),Eq(folder2.get_all_contents()));
}


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_save_into_a_new_folder)
{
    message.save(folder3);
    EXPECT_THAT(folder3.get_all_contents(),Contains(init_content));
}


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_delete_from_folder)
{
    message.remove_from(folder1);
    EXPECT_THAT(folder1.get_all_contents(),Not(Contains(init_content)));
}

TEST_F(A_Message_Saved_By_Two_Folders_Test,test_set_new_content_will_sync_in_folders)
{
    message.set_content(new_content);
    EXPECT_THAT(folder1.get_all_contents(),Contains(new_content));
    EXPECT_THAT(folder2.get_all_contents(),Contains(new_content));
}


TEST_F(A_Message_Saved_By_Two_Folders_Test,test_destroied_message_will_be_removed_from_folders)
{
    //call destructor explicitly will cause undeifne behavior,don't do it
    std::string to_be_destructed_content("to_be_destructed!");
    {
        messge_and_folder::Message to_be_destructed_copy;
        to_be_destructed_copy.set_content(to_be_destructed_content);
        to_be_destructed_copy.save(folder3);
        EXPECT_THAT(folder3.get_all_contents(), Contains(to_be_destructed_content));
    }
    EXPECT_THAT(folder3.get_all_contents(),IsEmpty());
}


//////////////////////copy test//////////////////////////////////////
class A_Message_Saved_By_Two_Folders_That_Has_A_Copy_Test : public A_Message_Saved_By_Two_Folders_Test
{

    protected:
    virtual void SetUp() override
    {
        A_Message_Saved_By_Two_Folders_Test::SetUp();
        copy_message = message;
    }


    std::optional<messge_and_folder::Message> copy_message;
    std::string copy_content = "copy content!";
};

TEST_F(A_Message_Saved_By_Two_Folders_That_Has_A_Copy_Test,test_copied_message_has_same_content)
{
    EXPECT_THAT(copy_message->get_content(),Eq(message.get_content()));
}

TEST_F(A_Message_Saved_By_Two_Folders_That_Has_A_Copy_Test,test_copied_message_contained_in_same_folders)
{
    copy_message->set_content(copy_content);
    EXPECT_THAT(folder1.get_all_contents(),Contains(copy_content));
    EXPECT_THAT(folder2.get_all_contents(),Contains(copy_content));
}


TEST_F(A_Message_Saved_By_Two_Folders_That_Has_A_Copy_Test,test_copied_message_set_different_content_wont_affect_original)
{
    copy_message->set_content(copy_content);
    EXPECT_THAT(message.get_content(),Ne(copy_content));
}

TEST_F(A_Message_Saved_By_Two_Folders_That_Has_A_Copy_Test,test_copied_message_set_different_content_will_sync_in_folders)
{
    copy_message->set_content(copy_content);
    EXPECT_THAT(folder1.get_all_contents(),Contains(copy_content));
    EXPECT_THAT(folder2.get_all_contents(),Contains(copy_content));
}

////////////////////////override copy test/////////////////////////////////
class A_Message_Saved_By_Two_Folders_That_Has_A_Overrided_Copy_Test : public A_Message_Saved_By_Two_Folders_Test
{

    protected:
    virtual void SetUp() override
    {

        copy_message.save(folder3);
        A_Message_Saved_By_Two_Folders_Test::SetUp();
        copy_message = message;
    }

    messge_and_folder::Message copy_message;
};


TEST_F(A_Message_Saved_By_Two_Folders_That_Has_A_Overrided_Copy_Test,test_copy_is_contained_in_the_same_folders_as_original)
{
    EXPECT_THAT(folder1.get_all_contents(),UnorderedElementsAre(init_content,init_content));
    EXPECT_THAT(folder2.get_all_contents(),UnorderedElementsAre(init_content,init_content));
}


TEST_F(A_Message_Saved_By_Two_Folders_That_Has_A_Overrided_Copy_Test,test_copy_is_removed_from_its_original_folders)
{
    EXPECT_THAT(folder3.get_all_contents(),IsEmpty());
}
