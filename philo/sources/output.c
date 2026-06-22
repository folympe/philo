/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olfhal <olfhal@learner.42.tech>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 16:59:06 by olfhal            #+#    #+#             */
/*   Updated: 2026/05/25 16:59:08 by olfhal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_status_debug(t_philo *philo, char *color,
				char *str, t_status status)
{
	long	time;

	time = get_time_in_ms() - philo->table->start_time;
	if (status == GOT_FORK_1)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork [%d]\n",
			time, color, philo->id + 1, str, philo->fork[0]);
	else if (status == GOT_FORK_2)
		printf("[%10ld]\t%s%03d\t%s\e[0m: fork [%d]\n",
			time, color, philo->id + 1, str, philo->fork[1]);
	else
		printf("[%10ld]\t%s%03d\t%s\e[0m\n",
			time, color, philo->id + 1, str);
}

static void	write_status_debug(t_philo *philo, t_status status)
{
	if (status == DIED)
		print_status_debug(philo, RED, "died", status);
	else if (status == EATING)
		print_status_debug(philo, GREEN, "is eating", status);
	else if (status == SLEEPING)
		print_status_debug(philo, CYAN, "is sleeping", status);
	else if (status == THINKING)
		print_status_debug(philo, CYAN, "is thinking", status);
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		print_status_debug(philo, PURPLE, "has taken a fork", status);
}

static void	print_status(t_philo *philo, char *str)
{
	printf("%ld %d %s\n", get_time_in_ms() - philo->table->start_time,
		philo->id + 1, str);
}

void	write_status(t_philo *philo, bool reaper_report, t_status status)
{
	pthread_mutex_lock(&philo->table->write_lock);
	if (has_simulation_stopped(philo->table) && !reaper_report)
	{
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (DEBUG_FORMATTING)
	{
		write_status_debug(philo, status);
		pthread_mutex_unlock(&philo->table->write_lock);
		return ;
	}
	if (status == DIED)
		print_status(philo, "died");
	else if (status == EATING)
		print_status(philo, "is eating");
	else if (status == SLEEPING)
		print_status(philo, "is sleeping");
	else if (status == THINKING)
		print_status(philo, "is thinking");
	else if (status == GOT_FORK_1 || status == GOT_FORK_2)
		print_status(philo, "has taken a fork");
	pthread_mutex_unlock(&philo->table->write_lock);
}

void	write_outcome(t_table *table)
{
	unsigned int	i;
	unsigned int	full_count;
	unsigned int	target;

	full_count = 0;
	i = 0;
	target = (unsigned int)table->must_eat_count;
	while (i < table->nb_philos)
	{
		if (table->philos[i]->times_ate >= target)
			full_count++;
		i++;
	}
	pthread_mutex_lock(&table->write_lock);
	printf("%u/%u philosophers had at least %u meals.\n",
		full_count, table->nb_philos, target);
	pthread_mutex_unlock(&table->write_lock);
}
