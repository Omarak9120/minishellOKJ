/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 20:30:08 by mjamil            #+#    #+#             */
/*   Updated: 2024/12/24 20:30:11 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	append2(t_tokens **tmp, char *data, int type)
{
	t_tokens	*new_node;

	new_node = NULL;
	if ((*tmp)->id == TOKEN_SPACE && type == TOKEN_SPACE)
		return ; //for double space
	if (((*tmp)->id == TOKEN_COMMAND || (*tmp)->id == TOKEN_WORD)
		&& (type == TOKEN_WORD || type == TOKEN_COMMAND))
		(*tmp)->content = ft_strjoingnl((*tmp)->content, data);
	else if ((type != TOKEN_SPACE && (type == TOKEN_WORD
				|| type == TOKEN_COMMAND) && (*tmp)->id != TOKEN_SPACE))
	{
		new_node = newnode(ft_strdup(" "), TOKEN_SPACE);
		new_node->error = 0;
		(*tmp)->next = new_node;
		new_node->previous = (*tmp);
		(*tmp) = (*tmp)->next;
		addnode(tmp, data, type);
	}
	else
		addnode(tmp, data, type);
}

void	append(t_tokens **cmds, char *data, int type)
{
	t_tokens	*tmp;
	t_tokens	*new_node;
	char* 		dup_data= NULL;
	new_node = NULL;
	if ((!ft_strcmp(data, "\'\'") || !ft_strcmp(data, "\"\""))
		&& ft_strlen(data) == 2)
		{
 		    dup_data = ft_strdup("");
    		if (!dup_data)
        		return;
    		data = dup_data;
		}
		// printf("Command %s not found\n",data);
		// return;

	if (*cmds == NULL)
	{
		new_node = newnode(data, type);
		new_node->previous = newnode("START", TOKEN_START);
		new_node->error = 0;
		*cmds = new_node;
		if (dup_data)
			{
    			free(dup_data);
    			dup_data = NULL;
			}
		return ;
	}
	tmp = *cmds;
	while (tmp->next != NULL)
		tmp = tmp->next;
	append2(&tmp, data, type);
	if (dup_data)
			{
    			free(dup_data);
    			dup_data = NULL;
			}
}

t_token	get_delimiter_type(char *str)
{
	if (ft_strcmp(str, " ") == 0)
		return (TOKEN_SPACE);
	else if (ft_strcmp(str, "<<") == 0)
		return (TOKEN_HEREDOC_EOF);
	else if (ft_strcmp(str, ">>") == 0)
		return (TOKEN_OUT_A_FILE);
	else if (ft_strcmp(str, "<") == 0)
		return (TOKEN_IN_FILE);
	else if (ft_strcmp(str, ">") == 0)
		return (TOKEN_OUT_FILE);
	else if (ft_strcmp(str, "|") == 0)
		return (TOKEN_PIPE);
	return (TOKEN_WORD);
}

void	inittk(char *input, t_tk *tk, t_data *data)
{
	tk->i = 0;
	tk->buf_i = 0;
	tk->quote = 0;
	tk->last_was_space = 0;
	tk->foundcmd = 0;
	data->cmdchain = NULL;
	tk->buffer = ft_calloc(ft_strlen(input) + 1, sizeof(char));
}

void	printcmds(t_data *data)
{
	t_tokens	*tmp;
	const char	*type_names[] = {"DEFID", "WORD", "INFILE", "OUTFILE",
		"HEREDOC", "OUTAPPEND", "COMMAND", "OPERATOR", "PIPE", "SPACE",
		"FILE"};

	tmp = data->cmdchain;
	while (tmp != NULL)
	{
		printf("[%s] %s\n", type_names[tmp->id], tmp->content);
		tmp = tmp->next;
	}
}
