#include "libft.h"

int	write_nbr_fd(int n, int fd)
{
	int	count;

	count = 0;
	if (n == -2147483648)
	{
		ft_putstr_fd("-2147483648", fd);
		return (11);
	}
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		count++;
		n = -n;
	}
	if (n >= 10)
	{
		count += write_nbr_fd(n / 10, fd);
		count += write_nbr_fd(n % 10, fd);
	}
	else
	{
		ft_putchar_fd(n + '0', fd);
		count++;
	}
	return (count);
}
