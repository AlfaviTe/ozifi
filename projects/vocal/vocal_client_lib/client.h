#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <mutex>
#include <boost/optional.hpp>
#include <utils/buffer.h>
#include <library/udt/client.h>
#include <projects/vocal/vocal_client_lib/state.pb.h>

#include "callback.h"
#include "message.h"
#include "friend.h"
#include "conference.h"

namespace NVocal {

// Client

enum EClientState {
    CS_Disconnected,
    CS_Disconnecting,
    CS_Connecting,
    CS_ConnectingConfirmWait,
    CS_Logining,
    CS_LoginingConfirmWait,
    CS_Registering,
    CS_RegisteringConfirmWait,
    CS_Connected
};

typedef std::function<void(ELoginResult)> TLoginCallback;
typedef std::function<void(ERegisterResult)> TRegisterCallback;

struct TClientConfig {
    std::string SerializedState;                // internal state data, could be empty on first start
    TDataCallback StateChangeCallback;          // should be saved somewhere when changes
    TDataCallback CaptchaAvailableCallback;     // on captcha available (for login, reigster, etc.)
    TLoginCallback LoginResultCallback;         // on login failed / success
    TRegisterCallback RegisterResultCallback;   // on register success / fail
    TBoolCallback ConnectedCallback;            // on connection established / failed
    TNamedCallback CallCallback;                // on incoming call
    TNamedCallback ConferenceCallCallback;      // on incoming conference call
    TNamedCallback ConferenceJoinCallback;      // on join to conference
    TNamedCallback ConferenceLeftCallback;      // on conference left
    TMessageCallback MessageCallback;           // on message received
    TMessageCallback ConferenceMessageCallback; // on conference message received
    TMessageCallback FriendRequestCallback;     // on friend request received
};

class TClient {
public:
    TClient(const TClientConfig& config);
    EClientState GetState();

    // connection
    void Connect();                                     // initialize connection
    void Disconnect();                                  // initialize disconnection
    void Login(const std::string& login);               // initialize login
    void Login(const std::string& login,                // continue login
               const std::string& password,
               const std::string& captcha);
    void Register(const std::string& preferedLogin);    // initialize registration
    void Register(const std::string& preferedLogin,     // continue registration
                  const std::string& preferedPassword,
                  const std::string& email,
                  const std::string& captcha);

    // friends
    void AddFriend(const std::string& friendLogin);     // initialize adding friend
    void AddFriend(const std::string& friendLogin,      // continue adding friend
                   const std::string& requestMessage,
                   const std::string& captcha);
    void RemoveFriend(const std::string& friendLogin);  // remove friend from friendlist
    TFriend& GetFriend(const std::string& login);       // friend by login
    TFriendIterator FriendsBegin();                     // use it to iterate over friends
    TFriendIterator FriendsEnd();

    // conference
    void CreateConference();                            // init new conference creation
    void LeaveConference(const std::string& id);        // leaves conference
    TConference& GetConference(const std::string& id);  // conference by id
    TConferenceIterator ConferencesBegin();             // use it to iterate over conferences
    TConferenceIterator ConferencesEnd();
private:
    void OnConnected(bool success);
    void OnDataReceived(const TBuffer& data);
    void OnDisconnected();
    void ForceDisconnect();
private:
    EClientState CurrentState;
    TClientConfig Config;
    TFriends Friends;
    TConferences Conferences;
    std::unique_ptr<NUdt::TClient> Client;
    TClientState State;
    std::string Buffer;
    std::mutex Lock;
    std::string Password;
};

} // NVocal
