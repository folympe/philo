/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olfhal <olfhal@learner.42.tech>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 16:58:06 by olfhal            #+#    #+#             */
/*   Updated: 2026/05/25 16:58:08 by olfhal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static pthread_mutex_t	*init_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	unsigned int	i;

	forks = malloc(sizeof(pthread_mutex_t) * table->nb_philos);
	if (!forks)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < table->nb_philos)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			while (i--)
				pthread_mutex_destroy(&forks[i]);
			free(forks);
			return (error_null(STR_ERR_MUTEX, NULL, 0));
		}
		i++;
	}
	return (forks);
}

static void	assign_forks(t_philo *philo)
{
	unsigned int	left;
	unsigned int	right;

	left = philo->id;
	right = (philo->id + 1) % philo->table->nb_philos;
	if (philo->id % 2)
	{
		philo->fork[0] = right;
		philo->fork[1] = left;
	}
	else
	{
		philo->fork[0] = left;
		philo->fork[1] = right;
	}
}

static t_philo	**init_philosophers(t_table *table)
{
	t_philo			**philos;
	unsigned int	i;

	philos = malloc(sizeof(t_philo *) * table->nb_philos);
	if (!philos)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < table->nb_philos)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
			return (error_null(STR_ERR_MALLOC, NULL, 0));
		if (pthread_mutex_init(&philos[i]->meal_time_lock, NULL) != 0)
			return (error_null(STR_ERR_MUTEX, NULL, 0));
		philos[i]->table = table;
		philos[i]->id = i;
		philos[i]->times_ate = 0;
		assign_forks(philos[i]);
		i++;
	}
	return (philos);
}

static bool	init_global_mutexes(t_table *table)
{
	table->fork_locks = init_forks(table);
	if (!table->fork_locks)
		return (false);
	if (pthread_mutex_init(&table->sim_stop_lock, NULL) != 0)
		return (error_failure(STR_ERR_MUTEX, NULL, table));
	if (pthread_mutex_init(&table->write_lock, NULL) != 0)
		return (error_failure(STR_ERR_MUTEX, NULL, table));
	return (true);
}

t_table	*init_table(int ac, char **av, int i)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	table->nb_philos = ft_atoi(av[i++]);
	table->time_to_die = ft_atoi(av[i++]);
	table->time_to_eat = ft_atoi(av[i++]);
	table->time_to_sleep = ft_atoi(av[i++]);
	table->must_eat_count = -1;
	if (ac - 1 == 5)
		table->must_eat_count = ft_atoi(av[i]);
	table->sim_stop = false;
	if (!init_global_mutexes(table))
		return (NULL);
	table->philos = init_philosophers(table);
	if (!table->philos)
		return (NULL);
	return (table);
}
