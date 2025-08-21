/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:09:25 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/21 21:38:07 by ahamadi          ###   ########.fr       */
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
	if (philo->data->philo_count == 1)
	{
		print_status(philo, "has taken a fork");
		while (!is_simulation_end(philo->data))
			precise_usleep(1000, philo->data);
		return (NULL);
	}
	// if (philo->id % 2 == 0)
	// 	precise_usleep(1500, philo->data);
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
	long	last_meal;

	current_time = get_time();
	pthread_mutex_lock(&philo->data->death_mutex);
	last_meal = philo->last_meal_time;
	pthread_mutex_unlock(&philo->data->death_mutex);
	time_since_last_meal = current_time - last_meal;
	return (time_since_last_meal > philo->data->time_to_die);
	// // Debug output for death detection
	// if (time_since_last_meal > (philo->data->time_to_die * 0.8))
	// {
	// 	printf("⚠️  DEBUG: Philo %d is close to death! Time since last meal: %ld ms (limit: %ld ms)\n", 
	// 		philo->id, time_since_last_meal, philo->data->time_to_die);
	// }
	
	// if (time_since_last_meal > philo->data->time_to_die)
	// {
	// 	printf("💀 DEBUG: Philo %d DIED! Last meal: %ld ms, Current: %ld ms, Starved for: %ld ms\n", 
	// 		philo->id, last_meal, current_time, time_since_last_meal);
	// 	return (1);
	// }
	// return (0);
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
	int	count;

	if (data->must_eat_count == -1)
		return (0);
	i = 0;
	while (i < data->philo_count)
	{
		pthread_mutex_lock(&data->death_mutex);
		count = data->philosophers[i].eat_count;
		pthread_mutex_unlock(&data->death_mutex);
		if (count < data->must_eat_count)
			return (0);
		i++;
	}
	return (1);
}

/*
** Main monitoring function that runs in its own thread
** Args: void pointer to data structure
** Why: Continuously monitors for death or satisfaction conditions in parallel
** How: Check each philosopher in sequence, detect death/satisfaction
*/
void	*monitor_philosophers(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->philo_count)
		{
			if (is_philosopher_dead(&data->philosophers[i]))
			{
				print_status(&data->philosophers[i], "died");
				pthread_mutex_lock(&data->death_mutex);
				data->simulation_end = 1;
				pthread_mutex_unlock(&data->death_mutex);
				return (NULL);
			}
			i++;
		}
		if (all_philosophers_satisfied(data))
		{
			pthread_mutex_lock(&data->death_mutex);
			data->simulation_end = 1;
			pthread_mutex_unlock(&data->death_mutex);
			return (NULL);
		}
		usleep(1000);
	}
}

/*
** Starts the simulation by creating threads and monitoring
** Args: data structure
** Returns: SUCCESS or ERROR
** Why: Main simulation controller
** How: Create all philosopher threads and monitor thread, then join all
*/
int	start_simulation(t_data *data)
{
	int	i;

	// printf("🚀 SIMULATION START: %ld ms (absolute time)\n", data->start_time);
	// printf("📊 CONFIG: %d philosophers, die=%ldms, eat=%ldms, sleep=%ldms\n", 
	// 	data->philo_count, data->time_to_die, data->time_to_eat, data->time_to_sleep);
	// printf("----------------------------------------\n");
	i = 0;
	// if (1 == data->philo_count)
	// {
	// 	print_status(&data->philosophers[0], "has taken a fork");
	// 	precise_usleep(data->time_to_die, NULL);
	// 	print_status(&data->philosophers[0], "died");
	// 	return (SUCCESS);
	// }
	while (i < data->philo_count)
	{
		if (pthread_create(&data->philosophers[i].thread, NULL,
				philosopher_routine, &data->philosophers[i]) != 0)
			return (ERROR);
		i++;
	}
	if (pthread_create(&data->monitor_thread, NULL,
			monitor_philosophers, data) != 0)
		return (ERROR);
	pthread_join(data->monitor_thread, NULL);
	i = 0;
	while (i < data->philo_count)
	{
		pthread_join(data->philosophers[i].thread, NULL);
		i++;
	}
	return (SUCCESS);
}
