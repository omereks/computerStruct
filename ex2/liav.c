#include <stdio.h>
#include <string.h>

#define BOM1 (char)0xff
#define BOM2 (char)0xfe

void copy_file(char* src, char* dst, char* sign) {
    FILE *read;
    FILE *write;
    char ch[2];

    read = fopen(src, "rb");
    if (read == NULL) {
        printf("error in opening read file");
    }
    write = fopen(dst, "wb");
    if (write == NULL) {
        printf("error in opening write file");
    }
    unsigned long i;
    while (0 < (i = fread(ch,1,sizeof(ch),read))) {
        if (strcmp(sign, "keep") == 0) {
            fwrite(ch, 1, i, write);
        } else {
            char swap[2];
            swap[0] = ch[1];
            swap[1] = ch[0];
            fwrite(swap, 1, i, write);
        }
    }
    fclose(read);
    fclose(write);
}

int check_endian(const char* ch) {
    // 1 if little 0 if big
    if (ch[0] == BOM1 && ch[1] == BOM2) {
        return 1;
    }
    return 0;

}

void change_2(char* src, char* dst, char* src_os, char* dst_os, char* sign) {
    FILE *read = fopen(src, "rb");
    FILE *write = fopen(dst, "wb");
    char ch[2];

    if (read == NULL || write == NULL) {
        return;
    }
    unsigned long i = fread(ch, 1, sizeof(ch), read);
    int big_or_little = check_endian(ch);
    if (strcmp(sign, "keep") == 0) {
        if (strcmp(src_os, "-win") == 0) {
            while (0 < i) {
                if (big_or_little) {
                    if (ch[0] == 0x0d && ch[1] == 0x00) {
                        char b[2];
                        unsigned long x = fread(b, 1, sizeof(b), read);
                        if (b[0] == 0x0a && b[1] == 0x00) {
                            b[1] = 0x00;
                            if (strcmp(dst_os, "-unix") == 0) {
                                b[0] = 0x0a;
                            } else {
                                b[0] = 0x0d;
                            }
                            fwrite(b, 1, x, write);
                        } else {
                            fwrite(ch, 1, i, write);
                            fwrite(b, 1, x, write);
                        }
                    } else {
                        fwrite(ch, 1, i, write);
                    }
                } else {
                    if (ch[1] == 0x0d && ch[0] == 0x00) {
                        char b[2];
                        unsigned long x = fread(b, 1, sizeof(b), read);
                        if (b[1] == 0x0a && b[0] == 0x00) {
                            b[0] = 0x00;
                            if (strcmp(dst_os, "-unix") == 0) {
                                b[1] = 0x0a;
                            } else {
                                b[1] = 0x0d;
                            }
                            fwrite(b, 1, x, write);
                        } else {
                            fwrite(ch, 1, i, write);
                            fwrite(b, 1, x, write);
                        }
                    } else {
                        fwrite(ch, 1, i, write);
                    }
                }
                i = fread(ch, 1, sizeof(ch), read);
            }
            fclose(read);
            fclose(write);
        } else {
            while (0 < i) {
                if (big_or_little) {
                    if (strcmp(src_os, "-unix") == 0) {
                        if (ch[0] == 0x0a && ch[1] == 0x00) {
                            char b[4];
                            b[0] = 0x0d;
                            b[1] = 0x00;
                            b[2] = 0x0a;
                            b[3] = 0x00;
                            fwrite(b, 1, sizeof(b), write);
                        } else {
                            fwrite(ch, 1, i, write);
                        }
                    } else {
                        if (ch[0] == 0x0d && ch[1] == 0x00) {
                            char b[4];
                            b[0] = 0x0d;
                            b[1] = 0x00;
                            b[2] = 0x0a;
                            b[3] = 0x00;
                            fwrite(b, 1, sizeof(b), write);
                        } else {
                            fwrite(ch, 1, i, write);
                        }
                    }
                } else {
                    if (strcmp(src_os, "-unix") == 0) {
                        if (ch[1] == 0x0a && ch[0] == 0x00) {
                            char b[4];
                            b[0] = 0x00;
                            b[1] = 0x0d;
                            b[2] = 0x00;
                            b[3] = 0x0a;
                            fwrite(b, 1, sizeof(b), write);
                        } else {
                            fwrite(ch, 1, i, write);
                        }
                    } else {
                        if (ch[1] == 0x0d && ch[0] == 0x00) {
                            char b[4];
                            b[0] = 0x00;
                            b[1] = 0x0d;
                            b[2] = 0x00;
                            b[3] = 0x0a;
                            fwrite(b, 1, sizeof(b), write);
                        } else {
                            fwrite(ch, 1, i, write);
                        }
                    }
                }
                i = fread(ch, 1, sizeof(ch), read);
            }
            fclose(read);
            fclose(write);
        }
    } else {
        if (strcmp(src_os, "-win") == 0) {
            while (0 < i) {
                if (big_or_little) {
                    if (ch[0] == 0x0d && ch[1] == 0x00) {
                        char b[2];
                        unsigned long x = fread(b, 1, sizeof(b), read);
                        if (b[0] == 0x0a && b[1] == 0x00) {
                            b[0] = 0x00;
                            if (strcmp(dst_os, "-unix") == 0) {
                                b[1] = 0x0a;
                            } else {
                                b[1] = 0x0d;
                            }
                            fwrite(b, 1, x, write);
                        } else {
                            char swap[2];
                            swap[0] = ch[1];
                            swap[1] = ch[0];
                            fwrite(swap, 1, i, write);
                            fwrite(b, 1, x, write);
                        }
                    } else {
                        char swap[2];
                        swap[0] = ch[1];
                        swap[1] = ch[0];
                        fwrite(swap, 1, i, write);
                    }
                } else {
                    if (ch[1] == 0x0d && ch[0] == 0x00) {
                        char b[2];
                        unsigned long x = fread(b, 1, sizeof(b), read);
                        if (b[1] == 0x0a && b[0] == 0x00) {
                            b[1] = 0x00;
                            if (strcmp(dst_os, "-unix") == 0) {
                                b[0] = 0x0a;
                            } else {
                                b[0] = 0x0d;
                            }
                            fwrite(b, 1, x, write);
                        } else {
                            char swap[2];
                            swap[0] = ch[1];
                            swap[1] = ch[0];
                            fwrite(swap, 1, i, write);
                            fwrite(b, 1, x, write);
                        }
                    } else {
                        char swap[2];
                        swap[0] = ch[1];
                        swap[1] = ch[0];
                        fwrite(swap, 1, i, write);
                    }
                }
                i = fread(ch, 1, sizeof(ch), read);
            }
            fclose(read);
            fclose(write);
        } else {
            while (0 < i) {
                if (big_or_little) {
                    if (strcmp(src_os, "-unix") == 0) {
                        if (ch[0] == 0x0a && ch[1] == 0x00) {
                            char b[4];
                            b[0] = 0x00;
                            b[1] = 0x0d;
                            b[2] = 0x00;
                            b[3] = 0x0a;
                            fwrite(b, 1, sizeof(b), write);
                        } else {
                            char swap[2];
                            swap[0] = ch[1];
                            swap[1] = ch[0];
                            fwrite(swap, 1, i, write);
                        }
                    } else {
                        if (ch[0] == 0x0d && ch[1] == 0x00) {
                            char b[4];
                            b[0] = 0x00;
                            b[1] = 0x0d;
                            b[2] = 0x00;
                            b[3] = 0x0a;
                            fwrite(b, 1, sizeof(b), write);
                        } else {
                            char swap[2];
                            swap[0] = ch[1];
                            swap[1] = ch[0];
                            fwrite(swap, 1, i, write);
                        }
                    }
                } else {
                    if (strcmp(src_os, "-unix") == 0) {
                        if (ch[1] == 0x0a && ch[0] == 0x00) {
                            char b[4];
                            b[0] = 0x0d;
                            b[1] = 0x00;
                            b[2] = 0x0a;
                            b[3] = 0x00;
                            fwrite(b, 1, sizeof(b), write);
                        } else {
                            char swap[2];
                            swap[0] = ch[1];
                            swap[1] = ch[0];
                            fwrite(swap, 1, i, write);
                        }
                    } else {
                        if (ch[1] == 0x0d && ch[0] == 0x00) {
                            char b[4];
                            b[0] = 0x0d;
                            b[1] = 0x00;
                            b[2] = 0x0a;
                            b[3] = 0x00;
                            fwrite(b, 1, sizeof(b), write);
                        } else {
                            char swap[2];
                            swap[0] = ch[1];
                            swap[1] = ch[0];
                            fwrite(swap, 1, i, write);
                        }
                    }
                }
                i = fread(ch, 1, sizeof(ch), read);
            }
            fclose(read);
            fclose(write);
        }
    }
}

void change_1(char* src, char* dst, char* src_os, char* sign) {
    FILE *read = fopen(src, "rb");
    FILE *write = fopen(dst, "wb");
    char ch[2];

    if (read == NULL || write == NULL) {
        return;
    }
    unsigned long i = fread(ch, 1, sizeof(ch), read);
    int big_or_little = check_endian(ch);
    while (0 < i) {
        if (big_or_little) {
            if (strcmp(src_os, "-unix") == 0) {
                if (ch[0] == 0x0a && ch[1] == 0x00) {
                        ch[0] = 0x0d;
                        ch[1] = 0x00;
                }
            } else {
                if (ch[0] == 0x0d && ch[1] == 0x00) {
                    ch[0] = 0x0a;
                    ch[1] = 0x00;
                }
            }
        } else {
            if (strcmp(src_os, "-unix") == 0) {
                if (ch[0] == 0x00 && ch[1] == 0x0a) {
                    ch[0] = 0x00;
                    ch[1] = 0x0d;
                }
            } else {
                if (ch[0] == 0x00 && ch[1] == 0x0d) {
                    ch[0] = 0x00;
                    ch[1] = 0x0a;
                }
            }
        }
        if (strcmp(sign, "keep") == 0) {
            fwrite(ch,1,i,write);
        } else {
            char swap[2];
            swap[0] = ch[1];
            swap[1] = ch[0];
            fwrite(swap, 1, i, write);
        }
        i = fread(ch, 1, sizeof(ch), read);
    }
    fclose(read);
    fclose(write);
}

void change_ending(char* src, char* dst, char* src_os, char* dst_os, char* sign) {
    if (strcmp(src_os, dst_os) == 0) {
        copy_file(src, dst, sign);
        return;
    }
    if (strcmp(src_os, "-win") == 0 || strcmp(dst_os, "-win") == 0) {
        change_2(src, dst, src_os, dst_os, sign);
        return;
    }
    change_1(src, dst, src_os, sign);

}
int main(int argc, char* argv[]) {
    if (argc == 3) {
        copy_file(argv[1], argv[2], "keep");
        return 0;
    }
    if (argc == 5){
        change_ending(argv[1], argv[2], argv[3], argv[4], "keep");
        return 0;
    }
    if (argc == 6){
        change_ending(argv[1], argv[2], argv[3], argv[4], argv[5]);
        return 0;
    }
    return 0;
}

