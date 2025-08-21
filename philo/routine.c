/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:09:25 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/20 21:37:41 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Main philosopher thread routine
** Args: void pointer to philosopher structure
** Returns: NULL
** Why: Each philosopher runs this as their main thread function
** How: Continuous cycle of eating, sleeping, thinking until simulation ends
*/
void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (philo->id % 2 == 0)
		precise_usleep(15000);
	while (!is_simulation_end(philo->data))
	{
		philo_eat(philo);
		if (is_simulation_end(philo->data))
			break ;
		philo_sleep(philo);
		if (is_simulation_end(philo->data))
			break ;
		philo_think(philo);
	}
	return (NULL);
}

/*
** Checks if a philosopher has died from starvation
** Args: philosopher pointer
** Returns: 1 if dead, 0 if alive
** Why: Must detect death within 10ms as per subject requirements
*/
static int	is_philosopher_dead(t_philosopher *philo)
{
	long	current_time;
	long	time_since_last_meal;

	current_time = get_time();
	time_since_last_meal = current_time - philo->last_meal_time;
	return (time_since_last_meal > philo->data->time_to_die);
}

/*
** Checks if all philosophers have eaten required number of times
** Args: data structure
** Returns: 1 if all satisfied, 0 otherwise
** Why: Optional simulation end condition
*/
static int	all_philosophers_satisfied(t_data *data)
{
	int	i;

	if (data->must_eat_count == -1)
		return (0);
	i = 0;
	while (i < data->philo_count)
	{
		if (data->philosophers[i].eat_count < data->must_eat_count)
			return (0);
		i++;
	}
	return (1);
}

/*
** Main monitoring function that runs in the main thread
** Args: data structure
** Why: Continuously monitors for death or satisfaction conditions
** How: Check each philosopher in sequence, detect death/satisfaction
*/
void	monitor_philosophers(t_data *data)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < data->philo_count)
		{
			if (is_philosopher_dead(&data->philosophers[i]))
			{
				pthread_mutex_lock(&data->death_mutex);
				data->simulation_end = 1;
				pthread_mutex_unlock(&data->death_mutex);
				print_status(&data->philosophers[i], "died");
				return ;
			}
			i++;
		}
		if (all_philosophers_satisfied(data))
		{
			pthread_mutex_lock(&data->death_mutex);
			data->simulation_end = 1;
			pthread_mutex_unlock(&data->death_mutex);
			return ;
		}
		usleep(1000);
	}
}

/*
** Starts the simulation by creating threads and monitoring
** Args: data structure
** Returns: SUCCESS or ERROR
** Why: Main simulation controller
** How: Create all philosopher threads, then monitor from main thread
*/
int	start_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (pthread_create(&data->philosophers[i].thread, NULL,
				philosopher_routine, &data->philosophers[i]) != 0)
			return (ERROR);
		i++;
	}
	monitor_philosophers(data);
	i = 0;
	while (i < data->philo_count)
	{
		pthread_join(data->philosophers[i].thread, NULL);
		i++;
	}
	return (SUCCESS);
}
