# ifndef REFEREE
# define REFEREE

battle_field_t* battle_field;
typedef struct timespec walltime_t;

void walltime_start(walltime_t* start);
double walltime_elapsed(const walltime_t* start);
void fixed_sleep(useconds_t duration);
void startMapper();
void startPlayers();
void showResults(); // show winner and statistics
void endBattle(); // destroy threads
void startBattle();

#endif
