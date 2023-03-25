// Game controller

// Global state for game modes.
enum GameModes {
  MENU = 0,
  PLAYING = 1,
  TESTING = 2,
  RESET = 3,
} GameState;

unsigned char scores[4] = {0, 0, 0};

class Game {
  private:
    // Increment level every N shots.
    unsigned char every = 5;

  public:
    Game() {}

    void reset() {
      // Reset scores.
      scores[1] = 0;
      scores[2] = 0;
      scores[3] = 0;

      // Game start.
      GameState = PLAYING;
    }

    // Increases level based on scores.
    int getLevel() {
      int level = 1;
      // Add up all scores.
      unsigned char sum_scores;
      unsigned char p;
      for (p = 1; p < 4; p++) {
        sum_scores = sum_scores + scores[p];
      }
      // Increase level every 5 shots.
      for (p = 1; p < sum_scores; p++) {
        if (p % this->every == 0) {
          level++;
        }
      }
      return level;
    }
};

Game game;
