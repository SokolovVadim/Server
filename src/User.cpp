//
// Created by vadim on 11.10.20.
//

#include <string>
#include <iostream>
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <pwd.h>
#include <shadow.h>
#include <sys/wait.h>
#include <unistd.h>
#include <tuple>
#include <a.out.h>

enum USER
{
    PWD_LEN   = 2u << 5u,
    USR_LEN   = 2u << 5u,
    ENV_SIZE  = 2,
    E_LONG    = 1,
    E_FAILURE = 2
};

int CheckPassword( const char* user, const char* password );

class User
{
public:
    User():
        username_(""),
        password_(""),
        gid_(0),
        uid_(0)
    {}
    explicit User(std::string username):
        username_(std::move(username))
    {}

    ~User()
    {
        username_ = "";
        password_ = "";
    }

    /*
    You need to add the following (or equivalent) to the
    /etc/pam.d/check_user file:
    # check authorization
    auth       required     pam_unix.so
    account    required     pam_unix.so
    */

    struct pam_conv conv = {
            misc_conv,
            nullptr
    };

    bool validate(const std::string& username, const std::string& password) {
        pam_handle_t *pamh = nullptr;
        int retval(0);

        retval = pam_start("check_user", username.c_str(), &conv, &pamh);

        if (retval == PAM_SUCCESS)
            retval = pam_authenticate(pamh, 0);    /* is user really user? */

        if (retval == PAM_SUCCESS)
            retval = pam_acct_mgmt(pamh, 0);        

        /* This is where we have been authorized or not. */

        if (retval == PAM_SUCCESS) {
            fprintf(stdout, "Authenticated\n");
        } else {
            fprintf(stdout, "Not Authenticated\n");
        }

        if (pam_end(pamh,retval) != PAM_SUCCESS) {     /* close Linux-PAM */
            pamh = nullptr;
            fprintf(stderr, "check_user: failed to release authenticator\n");
            exit(1);
        }

        return retval != PAM_SUCCESS;       /* indicate success */
    }

    int sign_in(const std::string& username, const std::string& password)
    {
        if(username.length() > USR_LEN)
        {
            std::cerr << "Password is too long!\n";
            return E_LONG;
        }
        if(username.length() > PWD_LEN)
        {
            std::cerr << "Password is too long!\n";
            return E_LONG;
        }
        if(CheckPassword(username.c_str(), password.c_str())) {
            password_ = password;
        }
        else
        {
            std::cerr << "Password is invalid!\n";
            return E_FAILURE;
        }
        return 0;
    }

    int sign_up(const std::string& username, const std::string& password)
    {
        if(username.length() > USR_LEN)
        {
            std::cerr << "Password is too long!\n";
            return E_LONG;
        }
        if(username.length() > PWD_LEN)
        {
            std::cerr << "Password is too long!\n";
            return E_LONG;
        }
        std::cout << "username: " << username << ", password: " << password << std::endl;

        std::string cmd = "/bin/bash ./sign_up.sh " + username + " " + password;
        int sys_ret = system(cmd.c_str());
        if(sys_ret != 0)
        {
            std::cerr << "user has not been created!\n";
            return E_FAILURE;
        }
        password_ = password;
        return 0;
    }

    int get_credentials()
    {
        passwd pwd{};
        passwd *result = nullptr;
        char   *buf    = nullptr;
        ssize_t bufsize(0);


        bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
        if (bufsize == -1)          /* Value was indeterminate */
            bufsize = 16384;        /* Should be more than enough */

        buf = new char[bufsize];

        int ret_getpwnam_r = getpwnam_r(username_.c_str(), &pwd, buf, size_t(bufsize), &result);
        if (result == nullptr) {
            if (ret_getpwnam_r == 0)
                printf("Not found\n");
            else {
                errno = ret_getpwnam_r;
                perror("getpwnam_r");
            }
            exit(EXIT_FAILURE);
        }

        gid_ = pwd.pw_gid;
        uid_ = pwd.pw_uid;

        //printf("Name: %s; UID: %ld\n", pwd.pw_gecos, (long) pwd.pw_uid);
        return 0;
    }

    int run_client_routine(const std::pair<std::string, std::string> env[], const std::string & program, const std::string& args)
    {
        int* status(nullptr);
        int child = fork();
        if(child == 0) { // child
            setgid(gid_);
            setuid(uid_);

            for(int i(0); i < ENV_SIZE; ++i)
            {
                setenv(env[i].first.c_str(), env[i].second.c_str(), true);
            }
            execlp(program.c_str(), program.c_str(), args.c_str(), (char*)nullptr);
        } else if(child > 0) { // parent

            int ret_waitpid = waitpid(child, status, 0);
            if(ret_waitpid != 0)
            {
                perror("waitpid");
                return E_FAILURE;
            }
        } else {
            perror("fork");
            return E_FAILURE;
        }
        return 0;
    }


    int set_environment(std::pair<std::string, std::string> env[])
    {
        env[0] = {"HOME", "/home/" + username_};
        env[1] = {"USER", username_};

        return 0;
    }

private:
    std::string username_;
    std::string password_;
    gid_t gid_;
    uid_t uid_;
};

int sign_in(const std::string& username, const std::string& password)
{
    User user(username);
    if(user.sign_in(username, password) != 0)
    {
        return E_FAILURE;
    }
    if(user.get_credentials() != 0)
    {
        return E_FAILURE;
    }
    std::pair<std::string, std::string> env[ENV_SIZE] = {};
    user.set_environment(env);
    user.run_client_routine(env, "./server_routine", username);

    return 0;
}

int sign_up(const std::string& username, const std::string& password)
{
    User user(username);
    if(user.sign_up(username, password) != 0)
    {
        return E_FAILURE;
    }
    if(user.get_credentials() != 0)
    {
        return E_FAILURE;
    }
    std::pair<std::string, std::string> env[ENV_SIZE] = {};
    user.set_environment(env);
    user.run_client_routine(env, "./server_routine", username);
    return 0;
}

/// @return 0 - password is correct, otherwise no
int CheckPassword( const char* user, const char* password )
{
    struct passwd* passwdEntry = getpwnam( user );
    if ( !passwdEntry )
    {
        printf( "User '%s' doesn't exist\n", user );
        return 1;
    }

    if ( 0 != strcmp( passwdEntry->pw_passwd, "x" ) )
    {
        return strcmp( passwdEntry->pw_passwd, crypt( password, passwdEntry->pw_passwd ) );
    }
    else
    {
        // password is in shadow file
        struct spwd* shadowEntry = getspnam( user );
        if ( !shadowEntry )
        {
            printf( "Failed to read shadow entry for user '%s'\n", user );
            return 1;
        }

        return strcmp( shadowEntry->sp_pwdp, crypt( password, shadowEntry->sp_pwdp ) );
    }
}