# crackhead
A simple password cracker in C\
One way to encrypt a password is using a cryptographic hash function. As the name suggests, it is a hash function that takes an input and returns a fixed-size alphanumeric string. Ideally, it should be extremely computationally difficult to regenerate the password given only the hashed text. One example of the usage of such cryptographic hash function is password management in Operating Systems. Instead of storing the password in plain text, the OS encrypts (computes hash) and saves it. As a result, even if someone steals the password information (for instance /etc/shadow file containing the hashes of the password), he/she doesn’t immediately learn the passwords. For our project we would use the Ubuntu Linux distribution and it uses SHA-512 algorithm to store the encrypted passwords.

## Implementation
This project uses MPI to alphabtically divided password combinations to worker processes, and any remaning combinations are cracked by the master process.

## How to run
### macOS (Asli + Nakli)
```gcc ./crackhead_serial.c```

### Linux
1. Compile: ```gcc -lcrypt ./crackhead_serial.c cryptS```
2. Run:  ```sudo ./cryptS```

## Resources
### Saad
- https://codereview.stackexchange.com/questions/210656/c-program-to-crack-passwords

### Zohair
- https://devsrealm.com/cloud-computing/ubuntu/understanding-etc-passwd-etc-shadow-linux-ubuntu/
