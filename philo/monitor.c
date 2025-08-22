/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:00:00 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/22 15:02:23 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_philosopher_dead(t_philosopher *philo)
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
}

int	all_philosophers_satisfied(t_data *data)
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
