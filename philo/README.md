*This project has been created as part of the 42 curriculum by olfhal*

## PHILOSOPHERS

## Description
The Philosophers project at 42 School is a programming exercise based on the Dining Philosophers Problem, which introduces students to concurrency and synchronization. In this project, several philosophers sit around a table and alternate between eating, sleeping, and thinking. To eat, each philosopher must pick up two forks, which are shared resources placed between them. The challenge is to manage these resources using threads or processes without causing conflicts. Students must ensure that no philosopher dies from starvation and that the program runs smoothly. This requires the use of tools like mutexes or semaphores to control access to the forks. One of the main difficulties is avoiding deadlocks, where all philosophers are stuck waiting. Another challenge is preventing race conditions that could break the simulation. The project also involves handling precise timing for each action. Overall, it helps students understand how to write safe and efficient concurrent programs.

## Instructions
free_table:
*	Frees all of the memory allocated by the program.
*	Returns a NULL pointer if there is nothing to free,
*	or when all memory has been freed.

destroy_mutexts:
*	Destroys every mutex created by the program: fork locks, meal locks,
*	the write and simulation stopper lock.

msg:
*	Writes a message to the console. Returns the provided exit number.
*	Used for error management.

error_failure:
*	Frees any allocated memory, prints an error message and
*	returns 0 to indicate failure.
*	Used for error management during initialization.

error_null:
*	Frees any allocated memory, prints an error message and returns a NULL pointer.
*	Used for error management during initialization.

set_sim_stop_flag:
*	Sets the simulation stop flag to true or false. Only the grim
*	reaper thread can set this flag. If the simulation stop flag is
*	set to true, that means the simulation has met an end condition.

has_simulation_stopped:
*	Checks whether the simulation is at an end. The stop flag
*	is protected by a mutex lock to allow any thread to check
*	the simulation status without conflict.
*	Returns true if the simulation stop flag is set to true,
*	false if the flag is set to false. 

kill_philo:
*	Checks if the philosopher must be killed by comparing the
*	time since the philosopher's last meal and the time_to_die parameter.
*	If it is time for the philosopher to die, sets the simulation stop
*	flag and displays the death status.
*	Returns true if the philosopher has been killed, false if not.

end_condition_reached:
*	Checks each philosopher to see if one of two end conditions
*	has been reached. Stops the simulation if a philosopher needs
*	to be killed, or if every philosopher has eaten enough.
*	Returns true if an end condition has been reached, false if not.

grim_reaper:
*	The grim reaper thread's routine. Checks if a philosopher must
*	be killed and if all philosophers ate enough. If one of those two
*	end conditions are reached, it stops the simulation.

intit_forks:
*	Allocates memory and initializes fork mutexes.
*	Returns a pointer to the fork mutex array, or NULL if an error occured.

assign_forks:
*	Assigns two fork ids to each philosopher. Even-numbered philosophers
*	get their fork order switched. This is because the order in which
*	philosophers take their forks matters.
*
*	For example with 3 philos:
*		Philo #1 (id: 0) will want fork 0 and fork 1
*		Philo #2 (id: 1) will want fork 1 and fork 2
*		Philo #3 (id: 2) will want fork 2 and fork 0
*	If philo #1 takes fork 0, philo #2 takes fork 1 and philo #3 takes fork 2,
*	there is a deadlock. Each will be waiting for their second fork which is
*	in use by another philo.
*
*	Making even id philos "left-handed" helps:
*		Philo #1 (id: 0) takes fork 1 and then fork 0
*		Philo #2 (id: 1) takes fork 1 and then fork 2
*		Philo #3 (id: 2) takes fork 0 and then fork 2
*	Now, philo #1 takes fork 1, philo #3 takes fork 0 and philo #2 waits patiently.
*	Fork 2 is free for philo #3 to take, so he eats. When he is done philo #1 can
*	take fork 0 and eat. When he is done, philo #2 can finally get fork 1 and eat.

init_philosophers:
*	Allocates memory for each philosopher and initializes their values.
*	Returns a pointer to the array of philosophers or NULL if
*	initialization failed.

init_global_mutexes:
*	Initializes mutex locks for forks, writing and the stop simulation
*	flag.
*	Returns true if the initalizations were successful, false if
*	initilization failed.

init_table:
*	Initializes the "dining table", the data structure containing
*	all of the program's parameters.
*	Returns a pointer to the allocated table structure, or NULL if
*	an error occured during initialization.

start_simulation:
*	Launches the simulation by creating a grim reaper thread as well as
*	one thread for each philosopher.
*	Returns true if the simulation was successfully started, false if there
*	was an error. 

stop_simulation:
*	Waits for all threads to be joined then destroys mutexes and frees
*	allocated memory.

print_status_debug:
*	Prints the philosopher's status in an easier to read,
*	colorful format to help with debugging. For fork-taking
*	statuses, extra information is displayed to show which fork
*	the philosopher has taken.

write_status_debug:
*	Redirects the status writing for debug mode. For this option,
*	the DEBUG_FORMATTING option must be set to 1 in philo.h.

print_status:
*	Prints a philosopher's status in plain text as required by the project
*	subject:
*		timestamp_in_ms X status

write_status:
*	Prints the status of a philosopher as long as the simulation is
*	still active. Locks the write mutex to avoid intertwined messages
*	from different threads.
* 
*	If DEBUG_FORMATTING is set to 1 in philo.h, the status will
*	be formatted with colors and extra information to help with debugging.
*	Otherwise the output will be the regular format required by the project
*	subject.

write_outcome:
*	Prints the outcome of the simulation if a number of times to
*	eat was specified. Only used for debug purposes.

eat_sleep_routine:
*	When a philosopher is ready to eat, he will wait for his fork mutexes to
*	be unlocked before locking them. Then the philosopher will eat for a certain
*	amount of time. The time of the last meal is recorded at the beginning of
*	the meal, not at the end, as per the subject's requirements.

think_routine:
*	Once a philosopher is done sleeping, he will think for a certain
*	amount of time before starting to eat again.
*	The time_to_think is calculated depending on how long it has been
*	since the philosopher's last meal, the time_to_eat and the time_to_die
*	to determine when the philosopher will be hungry again.
*	This helps stagger philosopher's eating routines to avoid forks being
*	needlessly monopolized by one philosopher to the detriment of others.

lone_philo_routine:
*	This routine is invoked when there is only a single philosopher.
*	A single philosopher only has one fork, and so cannot eat. The
*	philosopher will pick up that fork, wait as long as time_to_die and die.
*	This is a separate routine to make sure that the thread does not get
*	stuck waiting for the second fork in the eat routine.

philosopher:
*	The philosopher thread routine. The philosopher must eat, sleep
*	and think. In order to avoid conflicts between philosopher threads,
*	philosophers with an even id start by thinking, which delays their
*	meal time by a small margin. This allows odd-id philosophers to
*	grab their forks first, avoiding deadlocks.

get_time_in_ms:
*	Gets the current time in miliseconds since the Epoch (1970-01-01 00:00:00).
*	Returns the time value.

philo_sleep:
*	Pauses the philosopher thread for a certain amount of time in miliseconds.
*	Periodically checks to see if the simulation has ended during the sleep
*	time and cuts the sleep short if it has.

/* sim_start_delay:
*	Waits for a small delay at the beginning of each threads execution
*	so that all threads start at the same time with the same start time
*	reference. This ensures the grim reaper thread is synchronized with
*	the philosopher threads.

### Resources
https://cdn.intra.42.fr/pdf/pdf/192339/en.subject.pdf
les studes de 42
miniliblx
l'ia m'a aider avec les normes 