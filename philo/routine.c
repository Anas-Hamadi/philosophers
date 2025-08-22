/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:09:25 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/22 15:02:46 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->data->death_mutex);
	if (philo->data->philo_count == 1)
	{
		print_status(philo, "has taken a fork");
		while (!is_simulation_end(philo->data))
			precise_usleep(1000, philo->data);
		return (NULL);
	}
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

static int	check_death_and_satisfaction(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (is_philosopher_dead(&data->philosophers[i]))
		{
			print_status(&data->philosophers[i], "died");
			pthread_mutex_lock(&data->death_mutex);
			data->simulation_end = 1;
			pthread_mutex_unlock(&data->death_mutex);
			return (1);
		}
		i++;
	}
	if (all_philosophers_satisfied(data))
	{
		pthread_mutex_lock(&data->death_mutex);
		data->simulation_end = 1;
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_philosophers(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (check_death_and_satisfaction(data))
			return (NULL);
		usleep(1000);
	}
}

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
