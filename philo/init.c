/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:08:09 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/21 01:21:43 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Initializes fork structures with mutexes
** Args: data structure containing fork array
** Returns: SUCCESS or ERROR
** Why: Each fork needs a mutex to prevent race conditions
*/
int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(t_fork) * data->philo_count);
	if (!data->forks)
		return (ERROR);
	i = 0;
	while (i < data->philo_count)
	{
		data->forks[i].id = i;
		if (pthread_mutex_init(&data->forks[i].mutex, NULL) != 0)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

/*
** Assigns fork pointers to philosophers using odd/even strategy
** Args: data structure
** Why: Prevents deadlock and ensures fairness
** How: Even philosophers pick left first, odd pick right first
** This breaks the circular wait condition that causes deadlock
*/
static void	assign_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (i % 2 == 0)
		{
			data->philosophers[i].left_fork = &data->forks[i];
			data->philosophers[i].right_fork = 
				&data->forks[(i + 1) % data->philo_count];
		}
		else
		{
			data->philosophers[i].left_fork = 
				&data->forks[(i + 1) % data->philo_count];
			data->philosophers[i].right_fork = &data->forks[i];
		}
		i++;
	}
}

int	init_philosophers(t_data *data)
{
	int	i;

	data->philosophers = malloc(sizeof(t_philosopher) * data->philo_count);
	if (!data->philosophers)
		return (ERROR);
	i = 0;
	while (i < data->philo_count)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].eat_count = 0;
		data->philosophers[i].last_meal_time = get_time(); // shouldn't last meal in init be 0 or something? 
		data->philosophers[i].data = data;
		i++;
	}
	assign_forks(data);
	return (SUCCESS);
}

/*
** Initializes all data structures and mutexes
** Args: data structure
** Returns: SUCCESS or ERROR
** Why: Must initialize everything before starting threads
*/
int	init_data(t_data *data)
{
	data->start_time = get_time();
	data->simulation_end = 0;
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (ERROR);
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
		return (ERROR);
	if (init_forks(data) == ERROR)
		return (ERROR);
	if (init_philosophers(data) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

/*
** Cleans up all allocated memory and destroys mutexes
** Args: data structure
** Why: Prevent memory leaks and properly destroy mutexes
*/
void	cleanup_data(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->philo_count)
		{
			pthread_mutex_destroy(&data->forks[i].mutex);
			i++;
		}
		free(data->forks);
	}
	if (data->philosophers)
		free(data->philosophers);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
}
