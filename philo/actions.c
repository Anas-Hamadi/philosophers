/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:08:21 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/22 14:59:33 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_forks(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->left_fork->mutex);
	if (is_simulation_end(philo->data))
	{
		pthread_mutex_unlock(&philo->left_fork->mutex);
		return (ERROR);
	}
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->right_fork->mutex);
	if (is_simulation_end(philo->data))
	{
		pthread_mutex_unlock(&philo->right_fork->mutex);
		pthread_mutex_unlock(&philo->left_fork->mutex);
		return (ERROR);
	}
	print_status(philo, "has taken a fork");
	return (SUCCESS);
}

static void	release_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(&philo->right_fork->mutex);
	pthread_mutex_unlock(&philo->left_fork->mutex);
}

void	philo_eat(t_philosopher *philo)
{
	if (take_forks(philo) == ERROR)
		return ;
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->last_meal_time = get_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->data->death_mutex);
	precise_usleep(philo->data->time_to_eat * 1000, philo->data);
	release_forks(philo);
}

void	philo_sleep(t_philosopher *philo)
{
	print_status(philo, "is sleeping");
	precise_usleep(philo->data->time_to_sleep * 1000, philo->data);
}

void	philo_think(t_philosopher *philo)
{
	long	think_time;

	print_status(philo, "is thinking");
	if (philo->data->philo_count % 2 == 1)
	{
		think_time = (philo->data->time_to_eat * 2)
			- philo->data->time_to_sleep;
		if (think_time > 0)
			precise_usleep(think_time * 1000, philo->data);
	}
}
