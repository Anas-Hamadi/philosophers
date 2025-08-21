/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:09:30 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/19 15:09:31 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Custom atoi implementation following 42 norm
** Returns: converted integer
** Why: Need string to int conversion for argument parsing
*/
int	ft_atoi(const char *str)
{
	int	result;
	int	i;

	result = 0;
	i = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result);
}

/*
** Gets current time in milliseconds since epoch
** Returns: time in milliseconds
** Why: Need precise timing for death detection and status messages
*/
long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/*
** Custom precise sleep function using busy waiting with small delays
** Args: microseconds to sleep
** Why: usleep() is inaccurate due to CPU scheduling - this is more precise
** How: Check time repeatedly with tiny usleep calls to avoid busy-waiting
*/
void	precise_usleep(long microseconds)
{
	long	start_time;
	long	current_time;
	long	elapsed;

	start_time = get_time() * 1000;
	while (1)
	{
		current_time = get_time() * 1000;
		elapsed = current_time - start_time;
		if (elapsed >= microseconds)
			break ;
		usleep(100);
	}
}

/*
** Thread-safe status printing with timestamp
** Args: philosopher pointer, status string
** Why: All status messages must be synchronized and timestamped
** How: Use print_mutex to prevent message overlap
*/
void	print_status(t_philosopher *philo, char *status)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->print_mutex);
	if (!is_simulation_end(philo->data))
	{
		timestamp = get_time() - philo->data->start_time;
		printf("%ld %d %s\n", timestamp, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}

/*
** Checks if simulation should end (thread-safe)
** Returns: 1 if simulation ended, 0 otherwise
** Why: Multiple threads need to check simulation state safely
*/
int	is_simulation_end(t_data *data)
{
	int	result;

	pthread_mutex_lock(&data->death_mutex);
	result = data->simulation_end;
	pthread_mutex_unlock(&data->death_mutex);
	return (result);
}
