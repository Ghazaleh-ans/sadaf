//* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_snprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gansari <gansari@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:20:40 by gansari           #+#    #+#             */
/*   Updated: 2025/04/28 12:20:42 by gansari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_snprintf(char c, t_snprintf_data *data)
{
	if (data->buffer && data->pos < data->bufsize - 1)
		data->buffer[data->pos] = c;
	data->pos++;
	data->written++;
}

void	ft_putstr_snprintf(char *s, t_snprintf_data *data)
{
	if (!s)
		s = "(null)";
	while (*s)
	{
		ft_putchar_snprintf(*s, data);
		s++;
	}
}

void	ft_typecheck_snprintf(va_list args, char *str, t_snprintf_data *data)
{
	if (*str == 'c')
		ft_putchar_snprintf(va_arg(args, int), data);
	else if (*str == 's')
		ft_putstr_snprintf(va_arg(args, char *), data);
	else if (*str == 'd' || *str == 'i')
		ft_putnbr_snprintf(va_arg(args, int), data);
	else if (*str == 'x')
		ft_putnbr_base_snprintf(va_arg(args, int), "0123456789abcdef", data);
	else if (*str == 'X')
		ft_putnbr_base_snprintf(va_arg(args, int), "0123456789ABCDEF", data);
	else if (*str == 'u')
		ft_putunsignednbr_snprintf(va_arg(args, unsigned int), data);
	else if (*str == 'p')
		ft_putptr_snprintf(va_arg(args, void *), data);
	else if (*str == '%')
		ft_putchar_snprintf(*str, data);
}

void	ft_null_terminate(t_snprintf_data *data)
{
	if (data->buffer && data->bufsize > 0)
	{
		if (data->pos >= data->bufsize)
			data->buffer[data->bufsize - 1] = '\0';
		else
			data->buffer[data->pos] = '\0';
	}
}

int	ft_snprintf(char *str, size_t size, const char *format, ...)
{
	va_list			args;
	t_snprintf_data	data;

	if (!format)
		return (0);
	data.buffer = str;
	data.bufsize = size;
	data.written = 0;
	data.pos = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			ft_typecheck_snprintf(args, (char *)format, &data);
		}
		else
			ft_putchar_snprintf(*format, &data);
		format++;
	}
	va_end(args);
	ft_null_terminate(&data);
	return (data.written);
}

// #include <stdio.h>
// #include <string.h>

// int main(void)
// {
//     char buffer1[50];
//     char buffer2[50];
//     int result1, result2;

//     // Test basic string formatting
//     result1 = ft_snprintf(buffer1, sizeof(buffer1), "Hello, %s!", "world");
//     result2 = snprintf(buffer2, sizeof(buffer2), "Hello, %s!", "world");
//     printf("ft_snprintf: [%s] (return: %d)\n", buffer1, result1);
//     printf("snprintf:    [%s] (return: %d)\n", buffer2, result2);

//     // Test with numbers
//     result1 = ft_snprintf(buffer1, sizeof(buffer1), "Value: %d, Hex: %x", 42, 42);
//     result2 = snprintf(buffer2, sizeof(buffer2), "Value: %d, Hex: %x", 42, 42);
//     printf("ft_snprintf: [%s] (return: %d)\n", buffer1, result1);
//     printf("snprintf:    [%s] (return: %d)\n", buffer2, result2);

//     // Test truncation (buffer too small)
//     result1 = ft_snprintf(buffer1, 10, "This string is too long to fit");
//     result2 = snprintf(buffer2, 10, "This string is too long to fit");
//     printf("ft_snprintf: [%s] (return: %d)\n", buffer1, result1);
//     printf("snprintf:    [%s] (return: %d)\n", buffer2, result2);

//     // Test with size of 0 (should just return the length that would have been written)
//     result1 = ft_snprintf(NULL, 0, "Test string %d", 123);
//     result2 = snprintf(NULL, 0, "Test string %d", 123);
//     printf("ft_snprintf with size 0: (return: %d)\n", result1);
//     printf("snprintf with size 0:    (return: %d)\n", result2);

//     return 0;
// }
