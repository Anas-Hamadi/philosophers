/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:08:21 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/21 17:09:22 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Handles fork acquisition in the correct order to prevent deadlock
** Args: philosopher pointer
** Returns: SUCCESS if both forks acquired, ERROR if simulation ended
** Why: Odd/even strategy prevents circular wait and ensures fairness
** How: Each philosopher has pre-assigned left/right based on their ID
*/
static int	take_forks(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->left_fork->mutex);
	if (is_simulation_end(philo->data))
	{
		pthread_mutex_unlock(&philo->left_fork->mutex);
		return (ERROR);
	}
	print_status(philo, "has taken a fork");
	if (philo->data->philo_count == 1)
	{
		pthread_mutex_unlock(&philo->left_fork->mutex);
		// maybe here set end_simulation to 1
		return (ERROR);
	}
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

/*
** Releases both forks in reverse order of acquisition
** Args: philosopher pointer
** Why: Good practice to release in reverse order
*/
static void	release_forks(t_philosopher *philo)
{
	pthread_mutex_unlock(&philo->right_fork->mutex);
	pthread_mutex_unlock(&philo->left_fork->mutex);
}

/*
** Philosopher eating action
** Args: philosopher pointer
** Why: Core action that prevents starvation
** How: Take forks, eat for specified time, update last meal time
*/
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

/*
** Philosopher sleeping action
** Args: philosopher pointer
** Why: Part of the philosopher lifecycle
** How: Print status and sleep for specified time
*/
void	philo_sleep(t_philosopher *philo)
{
	print_status(philo, "is sleeping");
	precise_usleep(philo->data->time_to_sleep * 1000, philo->data);
}

/*
** Philosopher thinking action
** Args: philosopher pointer
** Why: Part of the philosopher lifecycle and helps with timing
** How: Print status and think for a calculated time to maintain fairness
** The thinking time helps balance the simulation timing
*/
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
