
int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);

int main() {
    
    infof("INFO");
    warnf("WARNING");
    errorf("ERROR");
    panicf("PANIC");

    return 0;
}
