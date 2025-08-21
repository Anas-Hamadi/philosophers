/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:09:17 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/21 17:09:22 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>

# define MAX_PHILOS 200
# define SUCCESS 0
# define ERROR 1

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				id;
}	t_fork;

typedef struct s_data	t_data;

typedef struct s_philosopher
{
	int				id;
	int				eat_count;
	long			last_meal_time;
	t_fork			*left_fork;
	t_fork			*right_fork;
	pthread_t		thread;
	t_data			*data;
}	t_philosopher;

typedef struct s_data
{
	int				philo_count;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				must_eat_count;
	long			start_time;
	int				simulation_end;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_t		monitor_thread;
	t_philosopher	*philosophers;
	t_fork			*forks;
}	t_data;

/* Function prototypes */
int		parse_args(int argc, char **argv, t_data *data);
int		init_data(t_data *data);
int		init_philosophers(t_data *data);
int		init_forks(t_data *data);
void	cleanup_data(t_data *data);

void	*philosopher_routine(void *arg);
int		start_simulation(t_data *data);
void	*monitor_philosophers(void *arg);

void	philo_eat(t_philosopher *philo);
void	philo_sleep(t_philosopher *philo);
void	philo_think(t_philosopher *philo);

long	get_time(void);
void	precise_usleep(long microseconds, t_data *data);
void	print_status(t_philosopher *philo, char *status);
int		is_simulation_end(t_data *data);
int		ft_atoi(const char *str);

#endif
