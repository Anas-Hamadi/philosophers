/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamadi <ahamadi@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 15:08:33 by ahamadi           #+#    #+#             */
/*   Updated: 2025/08/21 17:26:08 by ahamadi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Validates that a string contains only digits and converts to positive int
** Returns: converted number if valid, -1 if invalid
** Why: Need to ensure all arguments are positive integers
*/
static int	validate_and_convert(char *str)
{
	int	i;
	int	result;

	i = 0;
	if (!str || !str[0])
		return (-1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		i++;
	}
	result = ft_atoi(str);
	if (result <= 0)
		return (-1);
	return (result);
}

int	parse_args(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
	{
		printf("Usage: %s number_of_philosophers time_to_die ", argv[0]);
		printf("time_to_eat time_to_sleep ");
		printf("[number_of_times_each_philosopher_must_eat]\n");
		return (ERROR);
	}
	data->philo_count = validate_and_convert(argv[1]);
	data->time_to_die = validate_and_convert(argv[2]);
	data->time_to_eat = validate_and_convert(argv[3]);
	data->time_to_sleep = validate_and_convert(argv[4]);
	if (data->philo_count == -1 || data->time_to_die == -1
		|| data->time_to_eat == -1 || data->time_to_sleep == -1)
		return (ERROR);
	if (argc == 6)
	{
		data->must_eat_count = validate_and_convert(argv[5]);
		if (data->must_eat_count == -1)
			return (ERROR);
	}
	else
		data->must_eat_count = -1;
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_data	data;

	memset(&data, 0, sizeof(t_data));
	if (parse_args(argc, argv, &data) == ERROR)
		return (ERROR);
	if (init_data(&data) == ERROR)
	{
		cleanup_data(&data);
		return (ERROR);
	}
	if (start_simulation(&data) == ERROR)
	{
		cleanup_data(&data);
		return (ERROR);
	}
	cleanup_data(&data);
	return (SUCCESS);
}
