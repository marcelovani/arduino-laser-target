class Runnable {
    static Runnable *headRunnable;
    Runnable *nextRunnable;

  public:
    Runnable() {
      nextRunnable = headRunnable;
      headRunnable = this;
    }

    virtual void setup() = 0;
    virtual void loop() = 0;
    virtual unsigned char priority() = 0;

    static void setupAll() {
      for (Runnable *r = headRunnable; r; r = r->nextRunnable)
        r->setup();
    }

    static void loopAll() {
      for (Runnable *r = headRunnable; r; r = r->nextRunnable) {
        // High priority classes runs more often.
        for (Runnable *p = headRunnable; p; p = p->nextRunnable) {
          if (p->priority() < 2) {
            p->loop();
          }
        }
        r->loop();
      }
    }

};

Runnable *Runnable::headRunnable;
