/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:09:30 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/22 15:03:09 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	long	result;
	int		i;

	result = 0;
	i = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		if (result > 2147483647)
			return (-1);
		i++;
	}
	return ((int)result);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	precise_usleep(long microseconds, t_data *data)
{
	long	start_time;
	long	current_time;
	long	elapsed;

	start_time = get_time() * 1000;
	if (NULL == data)
	{
		usleep(microseconds);
		return ;
	}
	while (1)
	{
		if (is_simulation_end(data))
			return ;
		current_time = get_time() * 1000;
		elapsed = current_time - start_time;
		if (elapsed >= microseconds)
			break ;
		usleep(100);
	}
}

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

int	is_simulation_end(t_data *data)
{
	int	result;

	pthread_mutex_lock(&data->death_mutex);
	result = data->simulation_end;
	pthread_mutex_unlock(&data->death_mutex);
	return (result);
}
