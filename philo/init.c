/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:08:09 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/22 15:00:41 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_forks(t_data *data)
{
	int	i;

	if (data->philo_count <= 0 || data->philo_count > 200)
		return (ERROR);
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

static void	assign_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_count)
	{
		if (i % 2 == 0)
		{
			data->philosophers[i].left_fork = &data->forks[i];
			data->philosophers[i].right_fork = &data->forks[(i + 1)
				% data->philo_count];
		}
		else
		{
			data->philosophers[i].left_fork = &data->forks[(i + 1)
				% data->philo_count];
			data->philosophers[i].right_fork = &data->forks[i];
		}
		i++;
	}
}

int	init_philosophers(t_data *data)
{
	int	i;

	if (data->philo_count <= 0 || data->philo_count > 200)
		return (ERROR);
	data->philosophers = malloc(sizeof(t_philosopher) * data->philo_count);
	if (!data->philosophers)
		return (ERROR);
	i = 0;
	while (i < data->philo_count)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].eat_count = 0;
		data->philosophers[i].last_meal_time = get_time();
		data->philosophers[i].data = data;
		i++;
	}
	assign_forks(data);
	return (SUCCESS);
}

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
