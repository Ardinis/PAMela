#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <security/pam_modules.h>
#include <security/pam_appl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MOUNT "secure_data-rw"
#define CONTAINER "secure_data_"

char    command[1000];

int	permissions(char const* username)
{
  memset(command, 0, 1000);
  if (sprintf(command, "chown %s:%s -R /home/%s/%s%s", username, username, username, CONTAINER ,username) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(command);

  if (sprintf(command, "chmod 700 -R /home/%s/%s%s", username, CONTAINER, username) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(command);

  if (sprintf(command, "chown %s:%s -R /home/%s/%s", username, username, username, MOUNT) == EXIT_FAILURE)
    return (!PAM_SUCCESS);

  system(command);
  if (sprintf(command, "chmod 700 -R /home/%s/%s", username, MOUNT) == EXIT_FAILURE)
    return (!PAM_SUCCESS);

  system(command);
  return (PAM_SUCCESS);
}

int	pam_decipher(char const* username)
{
  memset(command, 0, 1000);
  printf("Decryptage du container en cours\n", username);

  if (sprintf(command, "cryptsetup luksOpen /home/%s/%s%s %s%s",
	      username, CONTAINER, username, CONTAINER, username) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(command);

  if (sprintf(command, "mount -t ext4 /dev/mapper/%s%s /home/%s/%s",
	      CONTAINER, username, username, MOUNT) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(command);
  return (permissions(username));  
}

int	pam_cipher(char const* username)
{
  memset(command, 0, 1000);
  printf("Cryptage du container en cours\n", username);
  if (sprintf(command, "umount /home/%s/%s", username, MOUNT) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(command);

  if (sprintf(command, "cryptsetup luksClose %s%s",CONTAINER, username) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  system(command);

  return (permissions(username));
}

int	check_container(char const* username)
{
  memset(command, 0, 1000);
  if (sprintf(command, "/home/%s/secure_data_%s", username, username) == EXIT_FAILURE)
    return (!PAM_SUCCESS);
  if (open(command, O_RDONLY) == -1)
    {
      if (sprintf(command, "apt-get install cryptsetup") == EXIT_FAILURE)
	return (!PAM_SUCCESS);
      system(command);
      if (sprintf(command, "dd if=/dev/urandom bs=1M count=50 of=/home/%s/secure_data_%s", username, username) == EXIT_FAILURE)
      	return (!PAM_SUCCESS);
      system(command);
      if (sprintf(command, "cryptsetup luksFormat /home/%s/secure_data_%s", username, username) == EXIT_FAILURE)
	return (!PAM_SUCCESS);
      system(command);
      if (sprintf(command, "cryptsetup luksOpen /home/%s/secure_data_%s secure_data_%s", username, username, username) == EXIT_FAILURE)
	return (!PAM_SUCCESS);
      system(command);
      if (sprintf(command, "mkfs.ext4 /dev/mapper/secure_data_%s", username) == EXIT_FAILURE)
	return (!PAM_SUCCESS);
      system(command);
      if (sprintf(command, "mkdir /home/%s/secure_data-rw", username) == EXIT_FAILURE)
	return (!PAM_SUCCESS);
      system(command);
      if (sprintf(command, "echo '/dev/mapper/secure_data_%s /home/%s/secure_data-rw ext4 defaults, noauto 0 0' | tee --append /etc/fstab", username, username) == EXIT_FAILURE)
	return (!PAM_SUCCESS);
      system(command);
    }
  return (PAM_SUCCESS);
}

extern int	pam_sm_open_session(pam_handle_t *pamh, int flags, int argc, char const** argv)
{
  char	const* username;

  memset(command, 0, 1000);
  printf("Ouverture de la session\n");
  if (pam_get_user(pamh, &username, NULL) != PAM_SUCCESS)
    return (!PAM_SUCCESS);
  if (strcmp(username, "root") == 0)
    return (PAM_SUCCESS);
  if (check_container(username) != PAM_SUCCESS)
    return (!PAM_SUCCESS);
  (void)flags;
  (void)argc;
  (void)argv;
  return (pam_decipher(username));
}

extern int	pam_sm_close_session(pam_handle_t *pamh, int flags, int argc, char const** argv)
{
  char const* username;

  printf("Feremeture de la session\n");
  if (pam_get_user(pamh, &username, NULL) != PAM_SUCCESS)
    return (!PAM_SUCCESS);
  if (strcmp(username, "root") == 0)
    return (PAM_SUCCESS);
  (void)flags;
  (void)argc;
  (void)argv;
  return (pam_cipher(username));
}

extern int	pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, char const** argv)
{
  (void)pamh;
  (void)flags;
  (void)argc;
  (void)argv;
  return (PAM_SUCCESS);
}

extern int	pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, char const** argv)
{
  (void)pamh;
  (void)flags;
  (void)argc;
  (void)argv;
  return (PAM_SUCCESS);
}

extern int	pam_sm_chauthtok(pam_handle_t *pamh, int flags, int argc, char const** argv)
{
  (void)pamh;
  (void)flags;
  (void)argc;
  (void)argv;
  return (PAM_SUCCESS);
}

extern int	pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, char const** argv)
{
  (void)pamh;
  (void)flags;
  (void)argc;
  (void)argv;
  return (PAM_SUCCESS);
}

void            clean(pam_handle_t *pamh, void *data, int error_status)
{
  char          *xx;

  if ((xx = data))
    while (*xx)
      *xx++ = 0;
  free(data);
  (void)pamh;
  (void)error_status;
}