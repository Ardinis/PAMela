if [ "./installer.sh" = $0 ]
then
    if ! [ -f "/etc/pam.d/common-auth_bkp" ]
    then
        cp /etc/pam.d/common-auth /etc/pam.d/common-auth_bkp
        echo "\nauth optional pam_module.so\n" >> /etc/pam.d/common-auth
    fi
    if  ! [ -f "/etc/pam.d/common-session_bkp" ]
    then
        cp /etc/pam.d/common-session /etc/pam.d/common-session_bkp
        echo "\nsession optional pam_module.so\n" >> /etc/pam.d/common-session
    fi
    if ! [ -f "/etc/pam.d/common-account_bkp" ]
    then
        cp /etc/pam.d/common-account /etc/pam.d/common-account_bkp
        echo "\naccount optional pam_module.so\n" >> /etc/pam.d/common-account
    fi
    if ! [ -f "/etc/pam.d/common-password_bkp" ]
    then
        cp /etc/pam.d/common-password /etc/pam.d/common-password_bkp
        echo "\npassword optional pam_module.so\n" >> /etc/pam.d/common-password
    fi
    if ! [ -f "/lib/x86_64-linux-gnu/security/pam_module.so" ]
    then
        cp pam_module.so /lib/x86_64-linux-gnu/security/
    fi

    echo "Installation terminée"
    echo "\nsession optional pam_module.so\n" >> /etc/pam.d/common-session
    mv installer.sh uninstaller.sh
fi

if [ "./uninstaller.sh" = $0 ]
then
 mv /etc/pam.d/common-auth_bkp /etc/pam.d/common-auth
    mv /etc/pam.d/common-session_bkp /etc/pam.d/common-session
    mv /etc/pam.d/common-account_bkp /etc/pam.d/common-account
    mv /etc/pam.d/common-password_bkp /etc/pam.d/common-password
    rm /lib/x86_64-linux-gnu/security/pam_module.so

    echo "Désinstallation terminée"
    mv uninstaller.sh installer.sh
fi
