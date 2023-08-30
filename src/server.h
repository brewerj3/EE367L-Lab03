/// Handles sigchild
void sigchld_handler(int s);

/// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)